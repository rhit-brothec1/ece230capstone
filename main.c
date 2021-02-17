/*
 * Author:      Cooper Brotherton and Jesus Capo
 * Date:        February 12, 2021
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
#include <stdlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// Other includes
#include "lcd.h"
#include "inputs.h"
#include "outputs.h"
#include "delays.h"
#include "Timer.h"
#include "Tasks.h"

#define NUM_OF_TASKS                                                7

typedef enum _tasks
{
    Password, Lights, Temp, Direction, Power, Reaction, Binary
} Tasks;

typedef enum _diff
{
    Easy, Medium, Hard
} Difficulty;

static int digitalValue;
static volatile uint16_t taskIndex;
static volatile Tasks taskList[NUM_OF_TASKS];

Tasks currentTask;

/*!
 * \brief This function initializes the ADC14
 *
 * TODO
 *
 * \return None
 */
void ADC_init(void)
{
    // Enabling the FPU for floating point operation
    FPU_enableModule();
    FPU_enableLazyStacking();

    // Initializing ADC
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    // Configuring GPIOs (5.0, 5.1, 5.2; A5, A4, A3)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    // Configuring ADC
    ADC14_configureMultiSequenceMode(ADC_MEM3, ADC_MEM5, true);
    ADC14_configureConversionMemory(ADC_MEM3,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A3, false);
    ADC14_configureConversionMemory(ADC_MEM4,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A4, false);
    ADC14_configureConversionMemory(ADC_MEM5,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A5, false);
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    ADC14_setResolution(ADC_14BIT);
    ADC14_enableConversion();
    ADC14_enableInterrupt(ADC_INT3);
    ADC14_enableInterrupt(ADC_INT4);
    ADC14_enableInterrupt(ADC_INT5);
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

    srand(69);

    // TODO output_init and input_init
    outputs_init();
    inputs_init();
    ADC_init();
    Timer_init();

    const uint8_t port_mapping[] = {
    //Port P2: none, none, none, none, none, none, buzzer, servo
            PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE,
            PMAP_TA0CCR0A,
            PMAP_TA1CCR1A };

    PMAP_configurePorts((const uint8_t*) port_mapping, PMAP_P2MAP, 1,
    PMAP_DISABLE_RECONFIGURATION);

    // LCD initialization
    configLCD(GPIO_PORT_P3, GPIO_PIN3, GPIO_PORT_P3, GPIO_PIN2, GPIO_PORT_P6);
    initDelayTimer(CS_getMCLK());
    initLCD();

    Interrupt_enableMaster();
}

Difficulty setDifficulty(void)
{
    int select = 0;
    while (!Switch_pressed(1))
        ;
    commandInstruction(RETURN_HOME_MASK, false);
    printString("Easy            ", 16);
    while (!Switch_pressed(4))
    {
        if (Switch_pressed(1))
        {
            // Display new difficulty and rotate
            select = (select + 1) % 3;
            commandInstruction(RETURN_HOME_MASK, false);
            switch (select)
            {
            case 0:
                printString("Easy  ", 6);
                break;
            case 1:
                printString("Medium", 6);
                break;
            case 2:
                printString("Hard  ", 6);
                break;
            default:
                printString("Easy  ", 6);
            }

        }
        while (Switch_pressed(1))
            ;
    }

    return (Difficulty) select;
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

    printString("Welcome to\nEngineering Sim!", 27);
    delayMilliSec(5000);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    commandInstruction(RETURN_HOME_MASK, false);

    printString("Set difficulty:\nS1:select S2:set", 32);
    Difficulty difficulty = setDifficulty();
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);

    generateRandomOrder();

    currentTask = Power;
    taskDivertPower(&digitalValue);

// TODO game
    currentTask = taskList[taskIndex];
    Timer32_setCount(TIMER32_0_BASE, 60 * CS_getMCLK());
    Timer32_startTimer(TIMER32_0_BASE, true);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UPDOWN_MODE);

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

void T32_INT1_IRQHandler(void)
{
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    commandInstruction(RETURN_HOME_MASK, false);
    printString("Game over!", 10);
    GPIO_setOutputHighOnPin(BLINK_PORT, BLINK_PIN);
//    while (1)
//        ;
}
