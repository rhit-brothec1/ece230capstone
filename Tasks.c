/*
 * Tasks.c
 *
 *  Created on: Feb 16, 2021
 *      Author: Jesus Capo
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

void taskPassword()
{
    //Enter a password of min 4 length; incorrect decreases time; length of password increase with difficulty
    //TODO Wait for and get input from the keypad

    //TODO Check for win

    //TODO Check for FAILURE

}

void taskLights()
{
    //Cover PhotoR until a threshold value is reached relative to the initial value)
    //TODO Get the value of the photoR at the start of the task

    //TODO Check ADC input value from PhotoR

    //TODO Check for win

    //TODO Check for FAILURE

}

void taskTemp()
{
    //Hold temp sensor until a threshold value is reached (relative to the initial value)

    //TODO Get the value of the temperature at the start of the task

    //TODO Check ADC input value from Thermistor

    //TODO Check for win

    //TODO Check for FAILURE

}

void taskDirection()
{
    //Change angle of the servo to the desired position (or power level number) using the potentiometer,

    //TODO Reset the angle of the servo based on the current value from the potentiometer

    //TODO Get input from potentiometer

    //TODO Check for win

    //TODO Check for FAILURE

    //TODO Use potentiometer value to change the output of the servo

}

void taskDivertPower(int *digitalValue)
{
    //Use the potentiometer to "divert the power" to a certain value

    //TODO Get the value from the pot at the start of the task and DEFINE win condition based on the current value (i.e. X away from the start value)
    ADC14_toggleConversionTrigger();
    int target = rand() % 16384;

    while ((target < *digitalValue + 2000) && (target > *digitalValue - 2000))
    {
        target = rand() % 16384;
    }

    char a[5];
    sprintf(a, "%i", target);
    printString(a, 5);

    while (1)
    {
        //DONE Check for win
        if ((*digitalValue < target + 250) && (*digitalValue > target - 250))
        {
            printString("Game over!", 10);
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

void taskReaction()
{
    //Press button when BLUE LED is on (difficulty may change the color of the LED and may cause it to flash, fail if click at wrong time

    //TODO Check for win

    //TODO Check for failure
}

void taskBinary()
{
    //Task description
}
