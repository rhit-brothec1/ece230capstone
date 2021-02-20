/*
 * outputs.h
 *
 * Description: Header file for output functions. Includes several functions for
 *              LED2 and a function for the external LEDs and servo.
 *
 *   Edited on: Feb 19, 2021
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
#define TEN_DEGREES                                                 164

/*!
 * \brief This function initializes all of the outputs for the system
 *
 * This function initializes LED2 on P2, the external LEDs on P3, and servo.
 *
 * TA1.1 generates a PWM signal for the servo.
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
 * hexadecimal input. For example, a hexadecimal value of A (1010) will turn on
 * LEDs 4 and 2.
 *
 *  \param value is the decimal value the represents which LEDs are on.
 *          Valid values range from 0 to 13.
 *
 * \return None
 */
extern void External_LED_turnOnHex(int value);

/*!
 * \brief This function changes the angle of the servo based on the angle
 *
 * This function adjusts the PWM signal sent to the servo and therefore its angle.
 * The larger the value, the larger the angle
 *
 * \param value is 14-bit ADC value. Every 910 corresponds to 10°.
 *          Valid values are 0 - 16384.
 *          - \b 0 - 910 0°
 *          - \b 911 - 1820 10°
 *          - \b 1821 - 2730 20°
 *          - \b 2731 - 3640 30°
 *          - \b 3641 - 4550 40°
 *          - \b 4551 - 5460 50°
 *          - \b 5461 - 6370 60°
 *          - \b 6371 - 7280 70°
 *          - \b 7281 - 8190 90°
 *          - \b 8191 - 9100 100°
 *          - \b 9101 - 10010 110°
 *          - \b 10011 - 10920 120°
 *          - \b 10921 - 11830 130°
 *          - \b 11831 - 12740 140°
 *          - \b 12741 - 13650 150°
 *          - \b 13651 - 14560 160°
 *          - \b 14561 - 15470 170°
 *          - \b 15471 - 16384 180°
 *
 * \return None
 */
extern void Servo_setAngle(int value);

#ifdef __cplusplus
}
#endif

#endif /* OUTPUTS_H_ */
