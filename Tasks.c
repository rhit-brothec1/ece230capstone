/*
 * Tasks.c
 *
 * Description: Helper file containing implementation of the tasks
 *
 *   Edited on: Feb 23, 2021
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
    Timer32_setCount(
            TIMER32_0_BASE,
            Timer32_getValue(TIMER32_0_BASE) - CS_getMCLK() * (1 + difficulty));
}

void taskPassword(int difficulty)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    const int length = 4 + difficulty;

    char *password = malloc(length);

    int i;
    for (i = 0; i < length; i++)
    {
        // generate random password, exclude '#' and '*'
        int row;
        int col;
        do
        {
            row = rand() % 4;
            col = rand() % 4;
        }
        while (row == 3 && (col == 0 || col == 2));
        password[i] = keypad_map[row][col];
    }

    printString("Enter password:", 15);

    commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
    printString(password, length);
    for (i = length; i < 8; i++)
        printChar(' ');
    // Enter password, must enter correct char to progress
    for (i = 0; i < length; i++)
    {
        while (password[i] != keypad_get_input())
        {
            decrementTimer(difficulty);
        }
        printChar(password[i]);
    }
}

void taskLights(int difficulty, int *digitalValue)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    ADC14_toggleConversionTrigger();
    int target = 16000 - 300 * (3 - difficulty);

    printString("Turn off the\nlights", 19);

    while (*digitalValue < target)
    {
        ADC14_toggleConversionTrigger();
        delayMilliSec(100);
    }

}

void taskTemp(int difficulty, int *digitalValue)
{
    ADC14_toggleConversionTrigger();
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    printString("Turn up the\nheat", 17);

    int target = *digitalValue - 350;
    while (*digitalValue > target)
    {
        ADC14_toggleConversionTrigger();
        delayMilliSec(100);
    }

}

void taskDirection(int difficulty, int *digitalValue)
{
    ADC14_toggleConversionTrigger();
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    printString("Set direction to\n", 17);
    // Set angle based on current pot position for maximum interaction
    bool lt = *digitalValue < 7280;
    int targetAngle = (lt ? 7280 + rand() % 7280 : 7280 - rand() % 7280) / 910;
    int currentAngle = *digitalValue / 910;
    char t[5];
    sprintf(t, "T:%i0", targetAngle);

    while (targetAngle != currentAngle)
    {
        // Adjust servo, poll value, update LCD
        Servo_setAngle(*digitalValue);
        currentAngle = *digitalValue / 910;
        commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
        char a[5];
        sprintf(a, "C:%i0", currentAngle);
        printString(t, 5);
        printChar(0b11011111); // Degree sign
        printString(a, 5);
        printChar(0b11011111);
        // check for overshoot
        if (lt && currentAngle > targetAngle)
        {
            decrementTimer(difficulty);
            delayMilliSec(200);
        }
        else if (!lt && currentAngle < targetAngle)
        {
            decrementTimer(difficulty);
            delayMilliSec(200);
        }

        ADC14_toggleConversionTrigger();
        delayMilliSec(100);
    }
}

void taskDivertPower(int difficulty, int *digitalValue)
{
    ADC14_toggleConversionTrigger();
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);

    // randomize target value not near current value
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
        // poll value, update LCD
        float analogValue = (*digitalValue * 3.3) / 16384;
        commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
        char a[16];
        sprintf(a, "T:%4.2fV C:%4.2fV", analogTarget, analogValue);
        printString(a, 16);

        // Check for overshoot
        if (lt && *digitalValue - 250 * (3 - difficulty) > target)
        {
            decrementTimer(difficulty);
        }
        else if (!lt && *digitalValue + 250 * (3 - difficulty) < target)
        {
            decrementTimer(difficulty);
        }

        complete = ((*digitalValue < target + 50 * (3 - difficulty))
                && (*digitalValue > target - 50 * (3 - difficulty)));

        ADC14_toggleConversionTrigger();
        delayMilliSec(100);
    }
}

void taskReaction(int difficulty)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    printString("Press button\nwhen ", 18);
    // Choose random LED
    const int LED = rand() % 4;
    switch (LED)
    {
    case 0:
        printString("Y LED on", 8);
        break;
    case 1:
        printString("B LED on", 8);
        break;
    case 2:
        printString("G LED on", 8);
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
            for (i = 0; i < 50000; i++)
            {
                if (switch_pressed(5))
                {
                    External_LED_turnOff();
                    return;
                }
            }
            External_LED_turnOff();
            for (i = 0; i < 30000; i++)
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
            for (i = 0; i < 30000; i++)
            {
                if (switch_pressed(5))
                {
                    if (current == LED)
                    {
                        External_LED_turnOff();
                        return;
                    }
                    else
                        decrementTimer(difficulty);
                }
            }
            External_LED_turnOff();
            for (i = 0; i < 20000; i++)
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
            for (i = 0; i < 25000; i++)
            {
                if (switch_pressed(5))
                {
                    if (current == LED)
                    {
                        External_LED_turnOff();
                        return;
                    }
                    else
                        decrementTimer(difficulty);
                }
            }
            External_LED_turnOff();
            for (i = 0; i < 10000; i++)
            {
                if (switch_pressed(5))
                    decrementTimer(difficulty);
            }
        }
    }
}

void taskBinary(int difficulty)
{
    commandInstruction(RETURN_HOME_MASK, false);
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    printString("Press the right\nhex number", 26);

    // Generate random hex value
    int val = rand() % 14;
    External_LED_turnOnHex(val);

    bool complete = false;
    while (!complete)
    {
        // poll keypad, check if correct
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
