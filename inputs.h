/*
 * inputs.h
 *
 * Description: Header file for game inputs. It has functions for switch and
 *              keypad inputs
 *
 *  Created on: Feb 19, 2021
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

static char keypad_map[4][4] =
        { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' },
          { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };

/*!
 * \brief This function initializes the inputs for the system
 *
 * This function initializes P1.1, P1.4, and P1.5 for switch inputs, P4.0 for
 * keypad I/O, and ADC14.
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
extern bool switch_pressed(int pin);

/*!
 * \brief This function retrieves input from the keypad
 *
 * This function rotates through the row (output) pins to determine if and what
 * is being pressed on the keypad.
 *
 * \return char corresponding to the keypad input
 */
extern char keypad_get_input(void);

#ifdef __cplusplus
}
#endif

#endif /* INPUTS_H_ */
