/*
 * Tasks.h
 *
 *  Created on: Feb 16, 2021
 *      Author: Jesus Capo
 */

#ifndef TASKS_H_
#define TASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//FUNCTION TASK PROTOTYPES
extern void taskPassword();
extern void taskLights();
extern void taskTemp();
extern void taskDirection();
extern void taskDivertPower(int *digitalValue);
extern void taskReaction();
extern void taskBinary();


#ifdef __cplusplus
}
#endif

#endif /* TASKS_H_ */
