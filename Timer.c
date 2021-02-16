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
    GPIO_setAsPeripheralModuleFunctionOutputPin(
                GPIO_PORT_P2,
                GPIO_PIN6,
                GPIO_PRIMARY_MODULE_FUNCTION);
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
    TIMER32_PERIODIC_MODE);
    Timer32_enableInterrupt(TIMER32_0_BASE);
    Interrupt_enableInterrupt(INT_T32_INT1);
    // Timer_A0 PWM config for speaker, initialized to off
    const Timer_A_PWMConfig compareConfig_PWM = {
    TIMER_A_CLOCKSOURCE_SMCLK,                          // SMCLK Clock Source
            TIMER_A_CLOCKSOURCE_DIVIDER_1,              // SMCLK/1 = 3MHz
            0,                                          // Tick period
            TIMER_A_CAPTURECOMPARE_REGISTER_0,          // Use CCR0
            TIMER_A_OUTPUTMODE_TOGGLE,                  // Toggle output bit
            0                                           // Duty Cycle
            };

    // Timer_A2 updown config
    const Timer_A_UpDownModeConfig upDownConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK,                          // SMCLK Clock Source
            TIMER_A_CLOCKSOURCE_DIVIDER_64,              // SMCLK/1 = 3MHz
            300000,                                     // No period
            TIMER_A_TAIE_INTERRUPT_ENABLE,             // Enable Timer interrupt
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,         // Enable CCR0 interrupt
            TIMER_A_DO_CLEAR                            // Clear value
            };

    Timer_A_generatePWM(TIMER_A0_BASE, &compareConfig_PWM);
    Timer_A_configureUpDownMode(TIMER_A2_BASE, &upDownConfig);
    Interrupt_enableInterrupt(INT_TA2_0);
    Interrupt_enableInterrupt(INT_TA2_N);
}
