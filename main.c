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

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    while(1)
    {
        
    }
}
