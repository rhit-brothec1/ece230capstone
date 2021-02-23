/*
 * Author:      Cooper Brotherton and Jesus Capo
 * Date:        February 23, 2021
 * Libraries:   GPIO, Timer32, Timer A, ADC14, from DriverLib
 */
/******************************************************************************
 * MSP432 Capstone Project ECE230 Winter 2020-2021
 *
 * Description: A game called Engineering Simulator where a user selects a
 *              difficulty and interacts with sensors to complete all the tasks
 *              before time runs out.
 *
 *                MSP432P401
 *         /|\ ------------------
 *          | |                  |
 *          --|RST      P6.4-6.7 |===> D4-7
 *            |             P3.3 |---> RS
 *       S1-->|P1.1         P3.2 |---> E
 *       S2-->|P1.4              |
 *       S3-->|P1.5         P1.0 |---> LED1
 *    Therm-->|P5.0 P3.0,3.5-3.7 |---> External LEDs
 *    Photo-->|P5.1         P2.6 |---> Buzzer
 *      Pot-->|P5.2         P2.7 |---> Servo
 *    C1-4===>|P4.4-4.7 P4.0-4.3 |===> R1-4
 *
 *******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Other includes */
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

static int digitalValue;
static volatile Tasks taskList[NUM_OF_TASKS];

Tasks currentTask;

/*!
 * \brief This function sets up the project
 *
 * This function initializes the inputs and outputs and performs the necessary
 * port mapping. Master interrupts are also enabled.
 *
 * \return None
 */
void setup(void)
{
    WDT_A_holdTimer();

    inputs_init();
    outputs_init();
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

/*!
 * \brief This function uses I/O for setting the difficulty
 *
 * The LCD will always display "S1:select S2:set" on the bottom row to show the
 * user the what inputs are expected and what they do. The top row of the LCD
 * will initially display "Set difficulty:", then the currently selected
 * difficulty after the first S1 press.
 *
 * This function uses S1 and S2 to determine the difficulty of the game. S1
 * presses will rotate through the difficulties and S2 will set the difficulty.
 *
 * \return select is the integer representing the difficulty the user selected
 */
int setDifficulty(void)
{
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    commandInstruction(RETURN_HOME_MASK, false);
    printString("Set difficulty:\nS1:select S2:set", 32);
    int select = 0;
    // Let users interact with the mechanic before letting them set
    while (!switch_pressed(1))
        ;
    commandInstruction(RETURN_HOME_MASK, false);
    printString("Easy            ", 16);
    while (switch_pressed(1))
        ;
    // Loop to change difficulty until S2 press
    while (!switch_pressed(4))
    {
        if (switch_pressed(1))
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
                select = 0;
            }

        }
        // Wait until S1 has been depressed
        while (switch_pressed(1))
            ;
    }

    return select;
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
    // array to keep track of which Tasks have been picked
    bool picked[NUM_OF_TASKS] = { false };
    int i;
    for (i = 0; i < NUM_OF_TASKS; i++)
    {
        int num = rand() % NUM_OF_TASKS;
        // Probe until Task not picked
        while (picked[num])
        {
            num = (num + 1) % NUM_OF_TASKS;
        }

        picked[num] = true;
        taskList[i] = (Tasks) num;
    }
}

/*!
 * \brief This function handles the game flow
 *
 * This function handles the flow and order of the game by calling initialization
 * functions, then starting the game and calling the appropriate Task function.
 * It also handles the game ending sequence.
 *
 * \return int
 */
int main(void)
{
    setup();

    /* ----- Game introduction ----- */
    printString("Welcome to\nEngineering Sim!", 27);
    delayMilliSec(5000);

    /* ----- Game setup ----- */
    const int difficulty = setDifficulty();
    srand(time(0));

    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    generateRandomOrder();

    /* ----- Gameplay ----- */
    // Start game timer and blink/buzzer timer
    Timer32_setCount(TIMER32_0_BASE, 60 * CS_getMCLK());
    Timer32_startTimer(TIMER32_0_BASE, true);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UPDOWN_MODE);

    // task completion loop
    int taskIndex;
    for (taskIndex = 0; taskIndex < NUM_OF_TASKS; taskIndex++)
    {
        currentTask = taskList[taskIndex];
        switch (currentTask)
        {
        case Password:
            taskPassword(difficulty);
            break;
        case Lights:
            taskLights(difficulty, &digitalValue);
            break;
        case Temp:
            taskTemp(difficulty, &digitalValue);
            break;
        case Direction:
            taskDirection(difficulty, &digitalValue);
            break;
        case Power:
            taskDivertPower(difficulty, &digitalValue);
            break;
        case Reaction:
            taskReaction(difficulty);
            break;
        case Binary:
            taskBinary(difficulty);
            break;
        default:
            commandInstruction(RETURN_HOME_MASK, false);
            commandInstruction(CLEAR_DISPLAY_MASK, false);
            printString("Error 404:\nTask not found", 25);
        }
    }
    // Game completed
    Timer32_haltTimer(TIMER32_0_BASE);
    Timer_A_stopTimer(TIMER_A0_BASE);
    Timer_A_stopTimer(TIMER_A2_BASE);
    GPIO_setOutputLowOnPin(BLINK_PORT, BLINK_PIN);
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    long score = TIMER32_1->VALUE * (1 + difficulty * 0.3) / 420;
    char sal[26];
    sprintf(sal, "Good job!\nSalary: $%6d", score);
    printString(sal, 26);
}

/*!
 * \brief This function handles analog inputs
 *
 * This function handles the analog inputs of the potentiometer, thermistor, and
 * photoresistor and updates digitalValue based on the current task.
 *
 * \return None
 */
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

/*!
 * \brief This function handles the interrupt of Timer32_0
 *
 * This function handles the game over mechanism of the game by displaying
 * "You're fired!" to the LCD, turning on P1.0, and locking the system up.
 *
 * \return None
 */
void T32_INT1_IRQHandler(void)
{
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    commandInstruction(RETURN_HOME_MASK, false);
    printString("You're fired!", 13);
    GPIO_setOutputHighOnPin(BLINK_PORT, BLINK_PIN);
    abort();
}
