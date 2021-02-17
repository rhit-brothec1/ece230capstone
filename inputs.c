/*
 * intputs.c
 *
 *  Created on: Feb 16, 2021
 *      Author: Cooper Brotherton and Jesus Capo
 */
#include <inputs.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

const char keypad_map[4][4] = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' },
                                { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };
/*!
 * \brief This function configures the switches as inputs
 *
 * This function configures P1.1 and P1.4 as input pins with pull-up resistors
 *
 * \return None
 */
void Switch_init(void)
{
    // set S1 and S2 pins as input using GPIO driver functions
    GPIO_setAsInputPinWithPullUpResistor(SWITCH_PORT, SWITCH_PINS);
}

/*!
 * \brief This function configures port P4 for keypad I/O
 *
 * TODO
 *
 * \return None
 */
void Keypad_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(KEYPAD_PORT, KEYPAD_INPUT_PINS);
    GPIO_setAsOutputPin(KEYPAD_PORT, KEYPAD_OUTPUT_PINS);
    GPIO_setOutputHighOnPin(KEYPAD_PORT, KEYPAD_OUTPUT_PINS);
}

void inputs_init(void)
{
    Switch_init();
    Keypad_init();
}

bool Switch_pressed(int pin)
{
    return GPIO_getInputPinValue(SWITCH_PORT, 1 << pin) == GPIO_INPUT_PIN_HIGH ?
            false : true;
}

char Keypad_input(void)
{
    int key_out;
    int row;
    while (1)
    {
        for (row = 0; row < 4; row++)
        {
            GPIO_setOutputHighOnPin(KEYPAD_PORT, KEYPAD_OUTPUT_PINS);
            GPIO_setOutputLowOnPin(KEYPAD_PORT, 1 << row);

            // Invert value?
            key_out = (P4->IN & 0b11110000) >> 4;
            if (key_out != 0b1111)
            {
                switch (key_out)
                {
                case 0b0111:
                    return keypad_map[row][0];
                case 0b1011:
                    return keypad_map[row][1];
                case 0b1101:
                    return keypad_map[row][2];
                case 0b1110:
                    return keypad_map[row][3];
                default:
                    return ' ';
                }
            }
        }

    }
}
