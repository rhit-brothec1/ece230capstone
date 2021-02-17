/*
 * Timer.h
 *
 *  Created on: Feb 12, 2021
 *      Author: Cooper Brotherton
 */

#ifndef TIMER_H_
#define TIMER_H_

#define BLINK_PORT                                                  GPIO_PORT_P1
#define BLINK_PIN                                                   GPIO_PIN0

#define BEEP                                                        1000

/*!
 * \brief TODO
 */
extern void Timer_init(void);

#endif /* TIMER_H_ */
