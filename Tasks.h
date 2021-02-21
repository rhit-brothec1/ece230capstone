/*
 * Tasks.h
 *
 * Description: Header file containing descriptions for the tasks
 *
 *   Edited on: Feb 19, 2021
 *      Author: Jesus Capo and Cooper Brotherton
 */

#ifndef TASKS_H_
#define TASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*!
 * \brief This function handles the Password task
 *
 * This function generates a random password whose length depends on the
 * difficulty. The user then presses buttons on the keypad, if the user enters
 * the password incorrectly, they lose time, if they enter it correctly, the
 * complete the task.
 *
 * \param difficulty the difficulty the game is running at
 *
 * \return None
 */
extern void taskPassword(int difficulty);

/*!
 * \brief This function handles the Lights task
 *
 * This function will tell the user to turn off the lights. The user must
 * decrease the amount of light the photoresistor senses below a threshold to
 * complete the task.
 *
 * \param difficulty the difficulty the game is running at
 * \param *digitalValue is the pointer to the analog input of the photoresistor
 *
 * \return None
 */
extern void taskLights(int difficulty, int *digitalValue);

/*!
 * \brief This function handles the Temperature task
 *
 * This function will tell the user to turn up the heat. The user must
 * increase the temperature the thermistor senses above a threshold to complete
 * the task.
 *
 * \param difficulty the difficulty the game is running at
 * \param *digitalValue is the pointer to the analog input of the thermistor
 *
 * \return None
 */
extern void taskTemp(int difficulty, int *digitalValue);

/*!
 * \brief This function handles the Direction task
 *
 * This function gives the user an angle to set the servo to. The user adjusts
 * the potentiometer to the desired angle. If they adjust the potentiometer too
 * far or the wrong direction, they lose time, if they set the potentiometer to a
 * correct value, they complete the task.
 *
 * \param difficulty the difficulty the game is running at
 * \param *digitalValue is the pointer to the analog input of the potentiometer
 *
 * \return None
 */
extern void taskDirection(int difficulty, int *digitalValue);

/*!
 * \brief This function handles the Power task
 *
 * This function gives the user a voltage to set the potentiometer to. The user
 * adjusts the potentiometer to the desired value. If they adjust the
 * potentiometer too far or the wrong direction, they lose time, if they set the
 * potentiometer to a correct value, they complete the task.
 *
 * \param difficulty the difficulty the game is running at
 * \param *digitalValue is the pointer to the analog input of the potentiometer
 *
 * \return None
 */
extern void taskDivertPower(int difficulty, int *digitalValue);

/*!
 * \brief This function handles the Reaction task
 *
 * This function instructs the user to press the button when an LED of a specific
 * color lights up. If the user presses the button when another LED is on or the
 * specified LED is off, they lose time, if they press the button when the
 * specified LED is on, they complete the task.
 *
 * \param difficulty the difficulty the game is running at
 *
 * \return None
 */
extern void taskReaction(int difficulty);

/*!
 * \brief This function handles the Binary task
 *
 * This function generates a random value between 0 to 13 and turns on the
 * external LEDs as a binary representation of the value. The user presses a
 * button on the keypad. If the value does not correspond to the converted
 * hexadecimal value, they lose time, it the value does correspond to the
 * converted hexadecimal value, they complete the task.
 *
 * \param difficulty the difficulty the game is running at
 *
 * \return None
 */
extern void taskBinary(int difficulty);


#ifdef __cplusplus
}
#endif

#endif /* TASKS_H_ */
