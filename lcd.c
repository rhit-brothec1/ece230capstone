/*!
 * lcd.c
 *
 *      Description: Helper file for LCD library. For Hitachi HD44780 parallel
 *      LCD in 4-bit mode.
 *
 *      Author: ece230
 *      Edited by: Cooper Brotherton
 *      Date: February 10, 2021
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "lcd.h"
#include "delays.h"

#define NONHOME_MASK        0xFC

#define LONG_INSTR_DELAY    2000
#define SHORT_INSTR_DELAY   50
#define PINS_FOUR_BIT       0xF0

uint_fast8_t RS_Port, EN_Port, DB_Port;
uint_fast16_t RS_Pin, EN_Pin;

void configLCD(uint_fast8_t rsPort, uint_fast16_t rsPin, uint_fast8_t enPort,
               uint_fast16_t enPin, uint_fast8_t dbPort)
{
    GPIO_setOutputLowOnPin(enPort, enPin);

    GPIO_setAsOutputPin(rsPort, rsPin);
    GPIO_setAsOutputPin(enPort, enPin);
    GPIO_setAsOutputPin(dbPort, PINS_FOUR_BIT);

    RS_Port = rsPort;
    EN_Port = enPort;
    DB_Port = dbPort;
    RS_Pin = rsPin;
    EN_Pin = enPin;
}

/*!
 * Delay method based on instruction execution time.
 *   Execution times from Table 6 of HD44780 data sheet, with buffer.
 *
 * \param mode RS mode selection
 * \param instruction Instruction/data to write to LCD
 *
 * \return None
 */
void instructionDelay(uint8_t mode, uint8_t instruction)
{
    if ((mode == DATA_MODE) || (instruction & NONHOME_MASK))
    {
        delayMicroSec(SHORT_INSTR_DELAY);
    }
    else
    {
        delayMicroSec(LONG_INSTR_DELAY);
    }
}

/*!
 * Function to write instruction/data to LCD.
 *
 * \param mode          Write mode: 0 - control, 1 - data
 * \param instruction   Instruction/data to write to LCD
 * \param init          Whether the instruction part of the first
 *                      initialization instructions
 *
 * \return None
 */
void writeInstruction(uint8_t mode, uint8_t instruction, bool init)
{
    GPIO_setOutputLowOnPin(DB_Port, PINS_FOUR_BIT);
    if (mode == DATA_MODE)
    {
        GPIO_setOutputHighOnPin(RS_Port, RS_Pin);
    }
    else
    {
        GPIO_setOutputLowOnPin(RS_Port, RS_Pin);
    }
    GPIO_setOutputHighOnPin(EN_Port, EN_Pin);
    GPIO_setOutputHighOnPin(DB_Port, instruction);
    delayMicroSec(1);
    GPIO_setOutputLowOnPin(EN_Port, EN_Pin);
    // 4-bit operation requires two writes to DB4-7
    if (!init)
    {
        GPIO_setOutputLowOnPin(DB_Port, PINS_FOUR_BIT);
        GPIO_setOutputHighOnPin(EN_Port, EN_Pin);
        GPIO_setOutputHighOnPin(DB_Port, instruction << 4);
        delayMicroSec(1);
        GPIO_setOutputLowOnPin(EN_Port, EN_Pin);
    }

    instructionDelay(mode, instruction);
}

void commandInstruction(uint8_t command, bool init)
{
    writeInstruction(CTRL_MODE, command, init);
}

/*!
 * Function to write data instruction to LCD.
 *
 * \param data ASCII value/data to write to LCD
 *
 * \return None
 */
void dataInstruction(uint8_t data)
{
    writeInstruction(DATA_MODE, data, false);
}

void initLCD(void)
{
    // Primary initialization for 4-bit mode
    // See Figure 24 in Hitachi HD44780 data sheet
    delayMilliSec(40);
    commandInstruction(0x30, true);
    delayMilliSec(5);
    commandInstruction(0x30, true);
    delayMicroSec(150);
    commandInstruction(0x30, true);
    delayMicroSec(SHORT_INSTR_DELAY);
    commandInstruction(0x20, true);
    delayMicroSec(SHORT_INSTR_DELAY);

    // 4-bit, 2-line, 5x8 font
    commandInstruction(FUNCTION_SET_MASK | N_FLAG_MASK, false);
    // Display off
    commandInstruction(DISPLAY_CTRL_MASK, false);
    // Display clear
    commandInstruction(CLEAR_DISPLAY_MASK, false);
    // Cursor increment and no shift
    commandInstruction(ENTRY_MODE_MASK | ID_FLAG_MASK, false);
    // Initialization complete, turn ON display
    commandInstruction(DISPLAY_CTRL_MASK | D_FLAG_MASK, false);
    delayMilliSec(5);
}

void printChar(char character)
{
    dataInstruction(character);
}

void printString(char *chars, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if (chars[i] == '\n')
        {
            commandInstruction(SET_CURSOR_MASK | LINE2_OFFSET, false);
            continue;
        }
        if (chars[i] != 0)
        {
            printChar(chars[i]);
        }
    }
}
