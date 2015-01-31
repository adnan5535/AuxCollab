/* 
 * File:   main.c
 * Author: Adnan
 *
 * Created on January 26, 2015, 6:28 PM
 */

#include <p18cxxx.h>
#include "J1939.h"
#include "ecocar.h"
#include <delays.h>
#pragma config OSC = IRCIO67    // Oscillator Selection Bit
#pragma config BOREN = OFF      // Brown-out Reset disabled in hardware and software
#pragma config WDT = OFF        // Watchdog Timer disabled (control is placed on the SWDTEN bit)



/*
 * 
 */

char InputRegister;
//list of bits
#define WIPER_SWITCH 0
#define HAZ_SWITCH 1
#define HORN_SWITCH 2
#define BLINK_L_SWITCH 3
#define BLINK_R_SWITCH 4
//bits 5 , 6 and 7 unused


 char ReadInputs()
 {
     //clear InputRegister
     InputRegister = 0;
     //check WIPER_SWITCH
     //check HAZ_SWITCH
     //check HORN_SWITCH
     //check BLINK_L_SWITCH
     //check BLINK_R_SWITCH
     return(InputRegister);
 }

 char OutputRegister;
 //list of bits
#define HORN_FET 0
#define BRK_L_HIGH 1
#define BRK_R_HIGH 2
#define BLINK_L 3
#define BLINK_R 4
//bits 5 , 6 7 unused

 void main ()
 {
    
 }
