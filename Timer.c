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

/*!
 * \brief This function handles the interrupt of TA2 CCR0
 *
 * This function turns makes the speaker beep and toggles the LED
 *
 *\return None
 */
void TA2_0_IRQHandler(void)
{
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_setCompareValue(TIMER_A0_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            BEEP);
    GPIO_setOutputHighOnPin(BLINK_PORT, BLINK_PIN);
}

/*!
 * \brief This function handles the interrupt of TA2 CCRN
 *
 * This function turns off the speaker, togles the LED, and adjusts the frequency
 * based on the time remaining.
 *
 * \return None
 */
void TA2_N_IRQHandler(void)
{
    Timer_A_clearInterruptFlag(TIMER_A2_BASE);
    Timer_A_setCompareValue(TIMER_A0_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            0);
    Timer_A_setCompareValue(TIMER_A2_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            TIMER32_1->VALUE / 3840);
    GPIO_setOutputLowOnPin(BLINK_PORT, BLINK_PIN);
}
