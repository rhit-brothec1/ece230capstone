/*
 * Author:      Cooper Brotherton and Jesus Capo
 * Date:        February 10, 2021
 * Libraries:   GPIO, Timer32, Timer A, ADC14, TODO from DriverLib
 */
/******************************************************************************
 * MSP432 Capstone Project ECE230 Winter 2020-2021
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 *******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// Other includes
#include "lcd.h"
#include "Switches.h"
#include "LED.h"
#include "delays.h"

#define NUM_OF_TASKS    7

typedef enum _tasks
{
    Password, Lights, Temp, Direction, Power, Reaction, Binary
} Tasks;

static volatile uint16_t digitalValue;
static volatile uint16_t taskIndex;
static volatile Tasks taskList[NUM_OF_TASKS];

Tasks currentTask;

void init_ADC(void)
{
    // ADC initialization
    // Enabling the FPU for floating point operation
    FPU_enableModule();
    FPU_enableLazyStacking();

    // Initializing ADC
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    // TODO change
    // Configuring GPIOs (5.0, 5.1, 5.2; A5, A4, A3)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    // Configuring ADC
    ADC14_configureMultiSequenceMode(ADC_MEM14, ADC_MEM15, false);
    ADC14_configureConversionMemory(ADC_MEM14,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A14, false);
    ADC14_configureConversionMemory(ADC_MEM15,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A15, false);
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    ADC14_setResolution(ADC_14BIT);
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
    ADC14_enableInterrupt(ADC_INT14);
    ADC14_enableInterrupt(ADC_INT15);
    Interrupt_enableInterrupt(INT_ADC14);
}

/*!
 * \brief This function sets up the project
 *
 * TODO
 *
 * \return None
 */
void setup(void)
{
    // Stop Watchdog
    WDT_A_holdTimer();

    Switch_init();
    LED_init();

    // Timer32 in One-shot mode
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
    TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_0_BASE, CS_getMCLK());
    Timer32_startTimer(TIMER32_0_BASE, true);

    // LCD initialization
    configLCD(GPIO_PORT_P3, GPIO_PIN3, GPIO_PORT_P3, GPIO_PIN2, GPIO_PORT_P6);
    initDelayTimer(CS_getMCLK());
    initLCD();

    Interrupt_enableMaster();
}

/*!
 * \brief This function puts the tasks in a random order
 *
 * This function generates a random number associated with a Task and adds it to
 * an array. If the Task is already in the array, it will probe for a new Task
 * until it finds one that has not been added.
 *
 * \return None
 */
void generateRandomOrder(void)
{
    taskIndex = 0;
    // array to keep track of which Tasks have been picked
    bool picked[NUM_OF_TASKS] = { false };
    int i;
    for (i = 0; i < NUM_OF_TASKS; i++)
    {
        int num = rand() % (NUM_OF_TASKS + 1);
        // Probe until Task not picked
        while (picked[num])
        {
            num = (num + 1) % NUM_OF_TASKS;
        }

        picked[num] = true;
        taskList[i] = (Tasks) num;
    }
}

int main(void)
{
    setup();

// TODO welcome to game

    generateRandomOrder();

// TODO game
    currentTask = taskList[taskIndex];

    while (1)
    {

    }
}

void ADC14_IRQHandler(void)
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);
    // Potentiometer
    if (ADC_INT3 & status)
    {
        // if direction or power
        if (currentTask == Direction || currentTask == Power)
        {
            digitalValue = ADC14_getResult(ADC_INT3);
        }
        else
        {
            ADC14_toggleConversionTrigger();
        }
    }
    // Thermistor
    if (ADC_INT4 & status)
    {
        if (currentTask == Temp)
        {

            digitalValue = ADC14_getResult(ADC_INT4);
        }
        else
        {
            ADC14_toggleConversionTrigger();
        }
    }
    // Photoresistor
    if (ADC_INT5 & status)
    {
        if (currentTask == Lights)
        {
            digitalValue = ADC14_getResult(ADC_INT5);
        }
        else
        {

            ADC14_toggleConversionTrigger();
        }
    }
}
