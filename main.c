/* 
 * File:   main.c
 * Author: Adnan and Reegan
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
     if(TRISCbits.TRISC4 == 1)
     {
         InputRegister |= (1 << WIPER_SWITCH);
     }
     else
     {
         InputRegister &= ~(1 << WIPER_SWITCH);
     }
     //check HAZ_SWITCH
     if(TRISCbits.TRISC5 == 1)
     {
         InputRegister |= (1 << HAZ_SWITCH);
     }
     else 
     {
         InputRegister &= ~(1 << HAZ_SWITCH);
     }
     //check HORN_SWITCH
     if(TRISBbits.TRISB0 == 1)
     {
         InputRegister |= (1 << HORN_SWITCH);
     }
     else 
     {
         InputRegister &= ~(1 << HORN_SWITCH);
     }
     //check BLINK_L_SWITCH
     if(TRISCbits.TRISC7 == 1)
     {
         InputRegister |= (1 << BLINK_L_SWITCH);
     }
     else 
     {
         InputRegister &= ~(1 << BLINK_L_SWITCH);
     }
     //check BLINK_R_SWITCH
     if(TRISCbits.TRISC6 == 1)
     {
         InputRegister |= (1 << BLINK_R_SWITCH);
     }
     else
     {
         InputRegister &= ~(1 << BLINK_R_SWITCH);
     }
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

void HornOn(){
    LATAbits.LATA0=1;
}

//////////////////THIS IS OUR INTERRUPT SERVICE ROUTINE/////////////////////////
/* SOME THINGS TO KEEP IN MIND :
    Comments on the same line as the pragma statements here can cause problems*/
#pragma interrupt isr
void isr()
{
    if (INTCONbits.INT0IE&&INTCONbits.INT0IF) //Horn input detected and horn turned on
    {
        HornOn();
    }
}



//////////////////THIS IS OUR INTERRUPT/////////////////////////
// This calls the interrupt service routine when the interrupt is called

#pragma code high_vector = 0x08

/* this 'function' can only be 8 bytes in length. This is why the code we want
 * to run with our interrupt will be put in the interrupt service routine   */
void high_interrupt(void){

    /* This is an assembly instruction. This efficiently calls our interrupt
     * service routine */
    _asm GOTO isr _endasm
}



/* There is actually a space at the end of this line and a line break after...
 * This is important for some strange reason */
#pragma code


void main ()
 {
     INTCONbits.INT0IE = 1; //enable int0
     INTCON2bits.INTEDG0 = 1; //rising edge
     INTCONbits.GIE = 1; //global interrupt enable

     while(1)
     {

     }

 }
