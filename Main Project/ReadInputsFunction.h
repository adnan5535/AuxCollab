/* 
 * File:   ReadInputsFunction.h
 * Author: Reegan
 *
 * Created on February 15, 2015, 1:36 PM
 */

#ifndef READINPUTSFUNCTION_H
#define	READINPUTSFUNCTION_H

extern int InputRegister; //global variable
//list of bits
#define WIPER_SWITCH 0
#define HAZ_SWITCH 1
#define HORN_SWITCH 2
#define BLINK_L_SWITCH 3
#define BLINK_R_SWITCH 4
#define BRK_SWITCH 5
//bit 6 unused
#define TOGGLE_BIT 7

//Function Protype Declarations for ReadInputsFunction.c
unsigned char ReadInputs(void);

#endif