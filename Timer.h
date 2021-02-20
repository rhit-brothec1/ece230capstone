/*
 * Timer.h
 *
 * Description: Header file for handling the game timer
 *
 *   Edited on: Feb 12, 2021
 *      Author: Cooper Brotherton
 */

#ifndef TIMER_H_
#define TIMER_H_

#define BLINK_PORT                                                  GPIO_PORT_P1
#define BLINK_PIN                                                   GPIO_PIN0

#define BEEP                                                        1000

/*!
 * \brief This function initializes the game timer
 *
 * This function initializes LED1, the buzzer using TimerA0.0, and Timer32
 * TimerA2_0, TimerA2_N, and Timer32 interrupts are enabled.
 *
 * \return None
 */
extern void Timer_init(void);

#endif /* TIMER_H_ */
