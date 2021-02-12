/*
 * Timer.c
 *
 *  Created on: Feb 12, 2021
 *      Author: Cooper Brotherton
 */
#include <Timer.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void Blink_LED_init(void)
{
    GPIO_setAsOutputPin(BLINK_PORT, BLINK_PIN);
    GPIO_setOutputLowOnPin(BLINK_PORT, BLINK_PIN);
}

void Timer_init(void)
{
    Blink_LED_init();
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
                       TIMER32_PERIODIC_MODE);
}
