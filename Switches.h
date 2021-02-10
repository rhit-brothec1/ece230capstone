/*
 * Switches.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Cooper Brotherton
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define SWITCH_PORT                                                 GPIO_PORT_P1
#define SWITCH_PIN                                                  0x0012

/*!
 * \brief This function configures the switches as inputs
 *
 * This function configures P1.1 and P1.4 as input pins with pull-up resistors
 *
 * \return None
 */
extern void Switch_init(void);

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
 *
 * \return true if the switch is pressed, false otherwise
 */
extern bool Switch_pressed(int pin);

#ifdef __cplusplus
}
#endif

#endif /* SWITCHES_H_ */
