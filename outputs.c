/*
 * outputs.c
 *
 *  Created on: Feb 16, 2021
 *      Author: Cooper Brotherton and Jesus Capo
 */
#include <outputs.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*!
 * \brief This function configures LED2 pins as output pins
 *
 * This function configures P2.0, P2.1, and P2.2 as output pins for the RGB
 * LED2, and initializes them to 'off'
 *
 * \return None
 */
void RGBLED_init(void)
{
    GPIO_setAsOutputPin(RGB_PORT, RGB_ALL_PINS);
    GPIO_setOutputLowOnPin(RGB_PORT, RGB_ALL_PINS);
}

void External_LED_init(void)
{
    GPIO_setAsOutputPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
    GPIO_setOutputLowOnPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
}

Timer_A_PWMConfig servo_PWMConfig = {
TIMER_A_CLOCKSOURCE_SMCLK,
                                      TIMER_A_CLOCKSOURCE_DIVIDER_2,
                                      SERVO_PERIOD,
                                      TIMER_A_CAPTURECOMPARE_REGISTER_1,
                                      TIMER_A_OUTPUTMODE_RESET_SET,
                                      MIDDLE_ANGLE };

void Servo_init(void)
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(SERVO_PORT,
    SERVO_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
    Timer_A_generatePWM(TIMER_A1_BASE, &servo_PWMConfig);
}

void outputs_init(void)
{
    RGBLED_init();
    External_LED_init();
    Servo_init();
}

void RGBLED_togglePin(int pin)
{
    GPIO_toggleOutputOnPin(RGB_PORT, 1 << pin);
}

void RGBLED_turnOff()
{
    GPIO_setOutputLowOnPin(RGB_PORT, RGB_ALL_PINS);
}

void RGBLED_turnOnOnlyPin(int pin)
{
    int mask = 1 << pin;
    GPIO_setOutputLowOnPin(RGB_PORT, ~mask);
    GPIO_setOutputHighOnPin(RGB_PORT, mask);
}

void External_LED_turnOnHex(int value)
{
    GPIO_setOutputLowOnPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
    // Values are weird b/c the LEDs are 3.0, 3.5, 3.6, 3.7
    switch (value)
    {
    case 0:
        break;
    case 1:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b00000001);
        break;
    case 2:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b00100000);
        break;
    case 3:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b00100001);
        break;
    case 4:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01000000);
        break;
    case 5:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01000001);
        break;
    case 6:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01100000);
        break;
    case 7:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01100001);
        break;
    case 8:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10000000);
        break;
    case 9:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10000001);
        break;
    case 10:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10100000);
        break;
    case 11:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10100001);
        break;
    case 12:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b11000000);
        break;
    case 13:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b11000001);
        break;
    }
}

void Servo_setAngle(int value)
{
    // TODO
}
