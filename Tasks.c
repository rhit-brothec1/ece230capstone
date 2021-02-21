/*
 * Tasks.c
 *
 * Description: Helper file containing implementation of the tasks
 *
 *   Edited on: Feb 19, 2021
 *      Author: Jesus Capo and Cooper Brotherton
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Other includes
#include "lcd.h"
#include "inputs.h"
#include "outputs.h"
#include "delays.h"
#include "Timer.h"
#include "Tasks.h"

/*!
 * \brief This function decrements the game timer
 *
 * This function decrements the game timer by one second and an additional second
 * for every difficulty level above Easy. It should be called only when a task is
 * done incorrectly.
 *
 * \param difficulty is the difficulty the game is being played at.
 *          - \b 0: Easy
 *          - \b 1: Medium
 *          - \b 2: Hard
 *
 * \return None
 */
void decrementTimer(int difficulty)
{
    Timer32_setCount(TIMER32_0_BASE,
                     Timer32_getValue(TIMER32_0_BASE) - (1 + difficulty));
}

void taskPassword(int difficulty)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    const int length = 4 + difficulty;

    char *password = malloc(length);
    char *input = malloc(length);

    int i;
    for (i = 0; i < length; i++)
    {
        password[i] = keypad_map[rand() % 4][rand() % 4];
    }

    printString("Enter password:", 15);

    bool complete = false;
    while (!complete)
    {
        complete = true;
        commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
        printString(password, length);
        for (i = length; i < 8; i++)
            printChar(' ');
        for (i = 0; i < length; i++)
        {
            input[i] = keypad_get_input();
            printChar(input[i]);
            if (input[i] != password[i])
                complete = false;
        }
        if (!complete)
            decrementTimer(difficulty);
    }

}

void taskLights(int difficulty, int *digitalValue)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    ADC14_toggleConversionTrigger();
    int target = *digitalValue - 2000;

    printString("Lights", 6);

//    while ((target < *digitalValue + 2000) && (target > *digitalValue - 2000))
//    {
//        target = rand() % 16384;
//    }

    char a[5];
    sprintf(a, "%i", target);
    printString(a, 5);

    while (1)
    {
        //DONE Check for win
        if ((*digitalValue < target + 250) && (*digitalValue > target - 250))
        {
            printString("Correct!", 8);
            return;
        }
        ADC14_toggleConversionTrigger();
        commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
        char a[5];
        sprintf(a, "%i", *digitalValue);
        printString(a, 5);
        delayMilliSec(1000);

        //TODO Check for FAILURE

    }

}

void taskTemp(int difficulty, int *digitalValue)
{
//Hold temp sensor until a threshold value is reached (relative to the initial value)

//TODO Get the value of the temperature at the start of the task
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    ADC14_toggleConversionTrigger();
    int target = rand() % 16384;

    while ((target < *digitalValue + 2000) && (target > *digitalValue - 2000))
    {
        target = rand() % 16384;
    }

    printString("Temp", 4);

    char a[5];
    sprintf(a, "%i", target);
    printString(a, 5);

    while (1)
    {
        //DONE Check for win
        if ((*digitalValue < target + 250) && (*digitalValue > target - 250))
        {
            printString("Correct!", 8);
            return;
        }
        ADC14_toggleConversionTrigger();
        commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
        char a[5];
        sprintf(a, "%i", *digitalValue);
        printString(a, 5);
        delayMilliSec(1000);

        //TODO Check for FAILURE

    }

}

void taskDirection(int difficulty, int *digitalValue)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    ADC14_toggleConversionTrigger();
    printString("Set direction to\n", 17);
    int angle = (
            *digitalValue < 7280 ? 7280 + rand() % 7280 : 7280 - rand() % 7280)
            / 910;

    bool complete = false;
    while (!complete)
    {
        Servo_setAngle(*digitalValue);
        if ((angle < *digitalValue + 910 * (2 - difficulty))
                && (angle > *digitalValue - 910 * (2 - difficulty)))
            complete = true;
        ADC14_toggleConversionTrigger();
    }
}

void taskDivertPower(int difficulty, int *digitalValue)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    ADC14_toggleConversionTrigger();
    int target = rand() % 16384;

    while ((target < *digitalValue + 2000) && (target > *digitalValue - 2000))
    {
        target = rand() % 16384;
    }

    bool lt = *digitalValue < target;

    float analogTarget = (target * 3.3) / 16384;
    printString("Set power to", 12);

    bool complete = false;
    while (!complete)
    {
        float analogValue = (*digitalValue * 3.3) / 16384;
        commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
        char a[16];
        sprintf(a, "T:%4.2fV C:%4.2fV", analogTarget, analogValue);
        printString(a, 16);

        if (lt && (*digitalValue - 250 * (3 - difficulty) > target))
        {
            decrementTimer(difficulty);
            commandInstruction(RETURN_HOME_MASK, false);
            printString("ooooooooooof", 12);
        }
        else if (!lt && *digitalValue + 250 * (3 - difficulty) < target)
        {
            decrementTimer(difficulty);
            commandInstruction(RETURN_HOME_MASK, false);
            printString("ooooooooooof", 12);
        }
        else
        {
            commandInstruction(RETURN_HOME_MASK, false);
            printString("Set power to", 12);
        }

        if ((*digitalValue < target + 250 * (3 - difficulty))
                && (*digitalValue > target - 250 * (3 - difficulty)))
        {
            complete = true;
        }
        ADC14_toggleConversionTrigger();
        delayMilliSec(250);

    }
}

void taskReaction(int difficulty)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    printString("Press button\nwhen ", 19);
    const int LED = rand() % 4;
    switch (LED)
    {
    case 0:
        printString("Y LED on", 8);
        break;
    case 1:
        printString("G LED on", 8);
        break;
    case 2:
        printString("B LED on", 8);
        break;
    case 3:
        printString("R LED on", 8);
        break;
    }

    switch (difficulty)
    {
    case 0:
        // Easy, only blink proper LED
        while (1)
        {
            External_LED_turnonLED(LED);
            int i;
            for (i = 0; i < 10000; i++)
            {
                if (switch_pressed(5))
                    return;
            }
            External_LED_turnOff();
            for (i = 0; i < 10000; i++)
            {
                if (switch_pressed(5))
                    decrementTimer(difficulty);
            }
        }
    case 1:
    {
        // Medium, blink two LEDs
        int other = (LED + 2) % 4;
        while (1)
        {
            int current = rand() % 2 == 1 ? other : LED;
            External_LED_turnonLED(current);
            int i;
            for (i = 0; i < 5000; i++)
            {
                if (switch_pressed(5))
                {
                    if (current == LED)
                        return;
                    else
                        decrementTimer(difficulty);
                }
            }
            External_LED_turnOff();
            for (i = 0; i < 5000; i++)
            {
                if (switch_pressed(5))
                    decrementTimer(difficulty);
            }
        }
    }
    case 2:
        // Hard, blink any LED quickly
        while (1)
        {
            int current = rand() % 4;
            External_LED_turnonLED(current);
            int i;
            for (i = 0; i < 5000; i++)
            {
                if (switch_pressed(5))
                {
                    if (current == LED)
                        return;
                    else
                        decrementTimer(difficulty);
                }
            }
            External_LED_turnOff();
            for (i = 0; i < 5000; i++)
            {
                if (switch_pressed(5))
                    decrementTimer(difficulty);
            }
            break;
        }
    }
    External_LED_turnOff();
}

void taskBinary(int difficulty)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    printString("Press the right\nhex number", 26);

    int val = rand() % 13;
    External_LED_turnOnHex(val);

    bool complete = false;
    while (!complete)
    {
        char input = keypad_get_input();
        switch (val)
        {
        case 0:
            complete = input == '0';
            break;
        case 1:
            complete = input == '1';
            break;
        case 2:
            complete = input == '2';
            break;
        case 3:
            complete = input == '3';
            break;
        case 4:
            complete = input == '4';
            break;
        case 5:
            complete = input == '5';
            break;
        case 6:
            complete = input == '6';
            break;
        case 7:
            complete = input == '7';
            break;
        case 8:
            complete = input == '8';
            break;
        case 9:
            complete = input == '9';
            break;
        case 10:
            complete = input == 'A';
            break;
        case 11:
            complete = input == 'B';
            break;
        case 12:
            complete = input == 'C';
            break;
        case 13:
            complete = input == 'D';
            break;
        }
        if (!complete)
            decrementTimer(difficulty);
    }
    External_LED_turnOff();
}
