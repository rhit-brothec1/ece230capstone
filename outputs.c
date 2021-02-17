/*
 * outputs.c
 *
 *  Created on: Feb 16, 2021
 *      Author: Cooper Brotherton and Jesus Capo
 */
#include <outputs.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void outputs_init(void)
{
    RGBLED_init();
    External_LED_init();
    Servo_init();
}
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
    // TODO convert P3 LEDs to hex
}
