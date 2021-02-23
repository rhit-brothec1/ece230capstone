/*
 * Timer.c
 *
 * Description: Helper file for handling the game timer
 *
 *   Edited on: Feb 19, 2021
 *      Author: Cooper Brotherton
 */
#include <Timer.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*!
 *  \brief This function initializes LED 1 (P1.0)
 *
 *  \return None
 */
void Blink_LED_init(void)
{
    GPIO_setAsOutputPin(BLINK_PORT, BLINK_PIN);
    GPIO_setOutputLowOnPin(BLINK_PORT, BLINK_PIN);
}

/*!
 * \brief This function initializes the buzzer
 *
 * This function sets P2.6 as a PWM output and sets up TA0.0 in PWM mode.
 * TimerA2_0 and TimerA2_N interrupts are enabled.
 *
 * \return None
 */
void Buzzer_init(void)
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
    GPIO_PIN6,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
    // Timer_A0 PWM config for speaker, initialized to off
    const Timer_A_PWMConfig compareConfig_PWM = {
            TIMER_A_CLOCKSOURCE_SMCLK,
            TIMER_A_CLOCKSOURCE_DIVIDER_1,
            0,
            TIMER_A_CAPTURECOMPARE_REGISTER_0,
            TIMER_A_OUTPUTMODE_TOGGLE, 0 };
    Timer_A_generatePWM(TIMER_A0_BASE, &compareConfig_PWM);
    Interrupt_enableInterrupt(INT_TA2_0);
    Interrupt_enableInterrupt(INT_TA2_N);
}

void Timer_init(void)
{
    Blink_LED_init();
    Buzzer_init();

    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
    TIMER32_FREE_RUN_MODE);
    Timer32_enableInterrupt(TIMER32_0_BASE);
    Interrupt_enableInterrupt(INT_T32_INT1);

    // Timer_A2 up down config for buzzer/LED toggle
    const Timer_A_UpDownModeConfig upDownConfig = {
            TIMER_A_CLOCKSOURCE_SMCLK,
            TIMER_A_CLOCKSOURCE_DIVIDER_64,
            300000,
            TIMER_A_TAIE_INTERRUPT_ENABLE,
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
            TIMER_A_DO_CLEAR };

    Timer_A_configureUpDownMode(TIMER_A2_BASE, &upDownConfig);
}

/*!
 * \brief This function handles the interrupt of TA2 CCR0
 *
 * This function turns makes the speaker beep and turns the LED on
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
 * This function turns off the speaker and LED and adjusts the frequency based on
 * the time remaining in Timer32.
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
                            Timer32_getValue(TIMER32_0_BASE) / 3840);
    GPIO_setOutputLowOnPin(BLINK_PORT, BLINK_PIN);
}
