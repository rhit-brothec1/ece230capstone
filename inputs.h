/*
 * inputs.h
 *
 *  Created on: Feb 16, 2021
 *      Author: Cooper Brotherton and Jesus Capo
 */

#ifndef INPUTS_H_
#define INPUTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define SWITCH_PORT                                                 GPIO_PORT_P1
#define SWITCH_PINS                                                 0x0032

#define KEYPAD_PORT                                                 GPIO_PORT_P4
#define KEYPAD_INPUT_PINS                                           0x00F0
#define KEYPAD_OUTPUT_PINS                                          0x000F

/*!
 * \brief This function initializes the inputs for the system
 *
 * TODO
 *
 * \return None
 */
extern void inputs_init(void);

/*!
 * \brief This function determines whether a switch is pressed
 *
 * This function gets the value at the pin and transforms it into a boolean to
 * convey whether or not the switch is pressed
 *
 * \param pin is the pin to check the input
 *          Valid values are:
 *          - \b 1 (P1.1 = S1)
 *          - \b 4 (P1.4 = S2)
 *          - \b 5 (P1.5 = Button)
 *
 * \return true if the switch is pressed, false otherwise
 */
extern bool Switch_pressed(int pin);

/*!
 * \brief This function retrieves input from the keypad
 *
 * TODO
 *
 * \return char corresponding to the keypad input
 */
extern char Keypad_input(void);

#ifdef __cplusplus
}
#endif

#endif /* INPUTS_H_ */
