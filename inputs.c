/*
 * intputs.c
 *
 * Description: Helper file for game outputs. Also includes more in-depth
 *              initialization.
 *
 *   Edited on: Feb 19, 2021
 *      Author: Cooper Brotherton and Jesus Capo
 */
#include <inputs.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*!
 * \brief This function configures the switches as inputs
 *
 * This function configures P1.1, P1.4, and P1.5 as input pins with pull-up
 * resistors.
 *
 * \return None
 */
void Switch_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(SWITCH_PORT, SWITCH_PINS);
}

/*!
 * \brief This function configures port P4 for keypad I/O
 *
 * This function initializes P4.0-4.3 as input pins with a pull-up resistor and
 * P4.4-4.7 as output pins.
 *
 * \return None
 */
void Keypad_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(KEYPAD_PORT, KEYPAD_INPUT_PINS);
    GPIO_setAsOutputPin(KEYPAD_PORT, KEYPAD_OUTPUT_PINS);
    GPIO_setOutputHighOnPin(KEYPAD_PORT, KEYPAD_OUTPUT_PINS);
}

/*!
 * \brief This function initializes the ADC14
 *
 * This function initializes the ADC14 module in 14-bit mode using A3-A5 with
 * interrupts. It also sets P5.0-5.2 as analog inputs.
 *
 * \return None
 */
void ADC_init(void)
{
    // Enabling the FPU for floating point operation
    FPU_enableModule();
    FPU_enableLazyStacking();

    // Initializing ADC
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    // Configuring GPIOs (5.0, 5.1, 5.2; A5, A4, A3)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    // Configuring ADC
    ADC14_configureMultiSequenceMode(ADC_MEM3, ADC_MEM5, true);
    ADC14_configureConversionMemory(ADC_MEM3,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A3, false);
    ADC14_configureConversionMemory(ADC_MEM4,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A4, false);
    ADC14_configureConversionMemory(ADC_MEM5,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A5, false);
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    ADC14_setResolution(ADC_14BIT);
    ADC14_enableConversion();
    ADC14_enableInterrupt(ADC_INT3);
    ADC14_enableInterrupt(ADC_INT4);
    ADC14_enableInterrupt(ADC_INT5);
    Interrupt_enableInterrupt(INT_ADC14);
}

void inputs_init(void)
{
    Switch_init();
    Keypad_init();
    ADC_init();
}

bool switch_pressed(int pin)
{
    return GPIO_getInputPinValue(SWITCH_PORT, 1 << pin) == GPIO_INPUT_PIN_HIGH ?
            false : true;
}

char keypad_get_input(void)
{
    int key_out;
    int row;
    while (1)
    {
        for (row = 0; row < 4; row++)
        {
            GPIO_setOutputHighOnPin(KEYPAD_PORT, KEYPAD_OUTPUT_PINS);
            GPIO_setOutputLowOnPin(KEYPAD_PORT, 1 << row);

            key_out = (P4->IN & 0b11110000) >> 4;
            if (key_out != 0b1111)
            {
                while ((P4->IN & 0b11110000) >> 4 != 0b1111)
                    ;
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
