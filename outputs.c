/*
 * outputs.c
 *
 * Description: Helper file for handling outputs. Also includes more in-depth
 *              initialization
 *
 *   Edited on: Feb 22, 2021
 *      Author: Cooper Brotherton and Jesus Capo
 */
#include <outputs.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*!
 * \brief This function configures the external LEDs
 *
 * This function configures P3.0 and P3.5-3.7 as output pins.
 *
 * \return None
 */
void External_LED_init(void)
{
    GPIO_setAsOutputPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
    GPIO_setOutputLowOnPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
}

Timer_A_PWMConfig servo_PWMConfig = { TIMER_A_CLOCKSOURCE_SMCLK,
                                      TIMER_A_CLOCKSOURCE_DIVIDER_2,
                                      SERVO_PERIOD,
                                      TIMER_A_CAPTURECOMPARE_REGISTER_1,
                                      TIMER_A_OUTPUTMODE_RESET_SET,
                                      MIDDLE_ANGLE };

/*!
 * \brief This function configures the servo
 *
 * This function sets P2.6 in PWM mode and generates a PWM signal using TimerA1.1
 *
 * \return None
 */
void Servo_init(void)
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(SERVO_PORT,
                                                SERVO_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
    Timer_A_generatePWM(TIMER_A1_BASE, &servo_PWMConfig);
}

void outputs_init(void)
{
    External_LED_init();
    Servo_init();
}

void External_LED_turnonLED(int LED)
{
    GPIO_setOutputLowOnPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
    const int mask = LED == 0 ? 1 : 1 << LED + 4;
    GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, mask);
}

void External_LED_turnOff(void)
{
    GPIO_setOutputLowOnPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
}

void External_LED_turnOnHex(int value)
{
    GPIO_setOutputLowOnPin(EXTERNAL_LED_PORT, EXTERNAL_LED_ALL_PINS);
    // Values are weird b/c the LEDs are 3.7, 3.6, 3.5, 3.0
    switch (value)
    {
    case 0:
        break;
    case 1:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b00000001);
        break;
    case 2:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b00100000);
        break;
    case 3:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b00100001);
        break;
    case 4:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01000000);
        break;
    case 5:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01000001);
        break;
    case 6:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01100000);
        break;
    case 7:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b01100001);
        break;
    case 8:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10000000);
        break;
    case 9:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10000001);
        break;
    case 10:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10100000);
        break;
    case 11:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b10100001);
        break;
    case 12:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b11000000);
        break;
    case 13:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b11000001);
        break;
    case 14:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b11100000);
        break;
    case 15:
        GPIO_setOutputHighOnPin(EXTERNAL_LED_PORT, 0b11100001);
        break;
    }
}

void Servo_setAngle(int value)
{
    int multiple = value / 910;
    servo_PWMConfig.dutyCycle = MAX_ANGLE - multiple * TEN_DEGREES;
    Timer_A_generatePWM(TIMER_A1_BASE, &servo_PWMConfig);
}
