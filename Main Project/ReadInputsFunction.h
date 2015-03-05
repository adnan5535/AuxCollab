/* 
 * File:   ReadInputsFunction.h
 * Author: Reegan
 *
 * Created on February 15, 2015, 1:36 PM
 */

#ifndef READINPUTSFUNCTION_H
#define	READINPUTSFUNCTION_H

extern unsigned char InputRegister; //global variable
//list of bits
#define WIPER_SWITCH 0
#define HAZ_SWITCH 1
#define HORN_SWITCH 2
#define BLINK_L_SWITCH 3
#define BLINK_R_SWITCH 4
#define BRK_SWITCH 5
//bit 6 unused

//Function Protype Declarations for ReadInputsFunction.c
unsigned char ReadInputs(void);

#endif