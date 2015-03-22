/* 
 * File:   Servo_Functions.h
 * Author: Reegan
 *
 * Created on February 15, 2015, 2:25 PM
 */

#ifndef SERVO_FUNCTIONS_H
#define	SERVO_FUNCTIONS_H

//constants specific to servo

#define CENTER_POSITION //probably won't need
#define REST_POSITION 105
//find rest position of the wiper
#define FAR_POSITION 50 //max wiper position
#define SERVO_RANGE 50 //max - rest
#define MAX_DEGREES 80  //furthest position of servo assuming WIPER_REST_POSTITION is zero degrees


//put function declarations here
void PWMSetup(char PWMInput); //call with PWMSetup(REST_POSITION)
unsigned char PWMUpdate(char PWMInput); //updates duty cycle (0 to 255)
void sweep(void); //sweeps servo back and forth once

#endif	/* SERVO_FUNCTIONS_H */

