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
#define REST_POSITION 15    //find rest position of the wiper for now its 50% duty cycle
#define EXTREME_POSITION 200 //max wiper position
#define SERVO_RANGE 185 //max - rest
#define MAX_DEGREES 180  //furthest position of servo assuming WIPER_REST_POSTITION is zero degrees


//put function declarations here
void PWMSetup(char PWMInput); //call with PWMSetup(REST_POSITION)
void PWMUpdate(char PWMInput); //updates duty cycle (0 to 255)
void PositionWiper(int Degrees);

#endif	/* SERVO_FUNCTIONS_H */
