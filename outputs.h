/*
 * outputs.h
 *
 *  Created on: Feb 16, 2021
 *      Author: Cooper Brotherton and Jesus Capo
 */

#ifndef OUTPUTS_H_
#define OUTPUTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define RGB_PORT                                                    GPIO_PORT_P2
#define RGB_ALL_PINS                                                0x0007
#define EXTERNAL_LED_PORT                                           GPIO_PORT_P3
#define EXTERNAL_LED_ALL_PINS                                       0x00E1

#define SERVO_PORT                                                  GPIO_PORT_P2
#define SERVO_PIN                                                   GPIO_PIN7
#define SERVO_PERIOD                                                37750
#define MIN_ANGLE                                                   700
#define MIDDLE_ANGLE                                                2176
#define MAX_ANGLE                                                   3652

/*!
 * \brief This function initializes all of the outputs for the system
 *
 * TODO
 *
 * \return None
 */
extern void outputs_init(void);

/*!
 * \brief This function toggles the LED at the given pin
 *
 * This function changes the state of the LED at the pin.
 *
 * \param pin is the pin to have its LED toggled
 *
 * \return None
 */
extern void RGBLED_togglePin(int pin);

/*!
 * \brief This function turns off LED2
 *
 * \return None
 */
extern void RGBLED_turnOff();

/*!
 * \brief This function turns on only one LED
 *
 * This function turns on the LED at the given pin and turns off the rest.
 *
 * \param pin is the pin which will have its LED on. All others will be off.
 *          Valid values are:
 *          - \b 0 (P2.0 = Red)
 *          - \b 1 (P2.1 = Green)
 *          - \b 2 (P2.2 = Blue)
 *
 * \return None
 */
extern void RGBLED_turnOnOnlyPin(int pin);

/*!
 * \brief This function turns on the external LEDs based on input value
 *
 * This function turns on the external LEDs to represent a binary value of the
 *  hexadecimal input. For example, a hexadecimal value of A (1010) will turn on
 *  LEDs 4 and 2.
 *
 *  \param value is the decimal value the represents which LEDs are on.
 *          Valid values range from 0 to 13.
 *
 * \return None
 */
extern void External_LED_turnOnHex(int value);

#ifdef __cplusplus
}
#endif

#endif /* OUTPUTS_H_ */
