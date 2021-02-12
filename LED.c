/*
 * LED.c
 *
 *  Created on: Dec 18, 2020
 *  Edited on:  Feb 10, 2021
 *      Author: Cooper Brotherton
 */

/* DriverLib Includes */
#include <LED.h>
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

void LED_init(void)
{
    RGBLED_init();
    External_LED_init();
}

void RGBLED_togglePin(int pin)
{
    MAP_GPIO_toggleOutputOnPin(RGB_PORT, 1 << pin);
}

void RGBLED_turnOff()
{
    MAP_GPIO_setOutputLowOnPin(RGB_PORT, RGB_ALL_PINS);
}

void RGBLED_turnOnOnlyPin(int pin)
{
    int mask = 1 << pin;
    MAP_GPIO_setOutputLowOnPin(RGB_PORT, ~mask);
    MAP_GPIO_setOutputHighOnPin(RGB_PORT, mask);
}

void External_LED_turnOnHex(int value)
{
    // TODO convert P3 LEDs to hex
}
