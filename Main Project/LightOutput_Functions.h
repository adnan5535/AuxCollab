/* 
 * File:   LightOutput_Functions.h
 * Author: Reegan
 *
 * Created on March 5, 2015, 9:52 AM
 */

#ifndef LIGHTOUTPUT_FUNCTIONS_H
#define	LIGHTOUTPUT_FUNCTIONS_H

#define ON 1 //note that this messes with config statements at start of main
#define OFF 0 //note that this messes with config statements at start of main
//fixed: put include statement after config statements
#define LEFT 1
#define RIGHT 2

extern unsigned char OutputRegister;
 //list of bits
#define HORN_FET 0
#define BRK_L_HIGH 1
#define BRK_R_HIGH 2
#define BLINK_L 3
#define BLINK_R 4
#define ERROR 5
#define SERVO 6
//bit 7 unused

void Error(unsigned char ErrorInput); //Turns Error led ON or OFF
void Haz(unsigned char HazInput); //Turns Hazard lights ON or OFF
void Horn(unsigned char HornInput); //Turns Horn ON or OFF
void Brake(unsigned char BrakeInput); //Turns Brake lights ON or OFF
void Signal(unsigned char SignalInput); //Signals LEFT or RIGHT or turns lights OFF







#endif	/* LIGHTOUTPUT_FUNCTIONS_H */

