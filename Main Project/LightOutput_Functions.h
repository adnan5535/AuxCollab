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
//bits 5 , 6 7 unused

void Haz(char HazInput);
void Horn(char HornInput);
void Brake(char BrakeInput);
void Signal(char SignalInput);







#endif	/* LIGHTOUTPUT_FUNCTIONS_H */

