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
#define ON 1
#define OFF 0

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
     INTCONbits.GIE = 0; //disable interrupts
     //clear InputRegister
     InputRegister = 0;
     //check WIPER_SWITCH
     if(PORTCbits.RC4 == 1)
     {
         InputRegister |= (1 << WIPER_SWITCH);
     }
     else
     {
         InputRegister &= ~(1 << WIPER_SWITCH);
     }
     //check HAZ_SWITCH
     if(PORTCbits.RC5 == 1)
     {
         InputRegister |= (1 << HAZ_SWITCH);
     }
     else 
     {
         InputRegister &= ~(1 << HAZ_SWITCH);
     }
     //check HORN_SWITCH
     if(PORTBbits.RB0 == 1)
     {
         InputRegister |= (1 << HORN_SWITCH);
     }
     else 
     {
         InputRegister &= ~(1 << HORN_SWITCH);
     }
     //check BLINK_L_SWITCH
     if(PORTCbits.RC7 == 1)
     {
         InputRegister |= (1 << BLINK_L_SWITCH);
     }
     else 
     {
         InputRegister &= ~(1 << BLINK_L_SWITCH);
     }
     //check BLINK_R_SWITCH
     if(PORTCbits.RC6 == 1)
     {
         InputRegister |= (1 << BLINK_R_SWITCH);
     }
     else
     {
         InputRegister &= ~(1 << BLINK_R_SWITCH);
     }
     INTCONbits.GIE = 1; //enable interrupts
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

void Horn(char Horn_Input) //turns horn on and off
{
    if (Horn_Input)
    {
        LATAbits.LATA0 = 1;
        OutputRegister |= (1 << HORN_FET);
    }
    else
    {
        LATAbits.LATA0 = 0;
        OutputRegister &= ~(1 << HORN_FET);
    }
}

void Brake(char Brake_Input) //turns brake lights on and off
{
    if(Brake_Input)
    {
        LATCbits.LATC0 = 1;
        LATCbits.LATC1 = 1;
        OutputRegister |= (1 << BRK_L_HIGH) | (1 << BRK_R_HIGH);
    }
    else
    {
        LATCbits.LATC0 = 0;
        LATCbits.LATC1 = 0;
        OutputRegister &= ~( (1 << BRK_L_HIGH) | (1 << BRK_R_HIGH) );
    }
}

void main ()
 {
     InitEcoCar(); //disables a bunch of stuff

     //set up input/ouputs
     TRISBbits.TRISB0 = 1;//HORN_SWITCH input
     TRISAbits.TRISA0 = 0; //HORN_FET output

     TRISBbits.TRISB1 = 1; //BRAKE_SWITCH input
     TRISCbits.TRISC0 = 0; //BRK_L_HIGH output
     TRISCbits.TRISC1 = 0; //BRK_R_HIGH output

     //other inputs
     TRISCbits.TRISC4 = 1;  //put description here
     TRISCbits.TRISC5 = 1;
     TRISCbits.TRISC6 = 1;
     TRISCbits.TRISC7 = 1;

     //other outputs


     //enable interrupt priorities
     RCONbits.IPEN = 1;

     //set up int0
     INTCONbits.INT0IE = 1; //enable int0     
     INTCONbits.INT0IF=0; //set flag to 0
     INTCON2bits.INTEDG0 = 1; //set to rising edge
     //is always high priority

     //setup int1
     INTCON3bits.INT1IE = 1; //enable
     INTCON3bits.INT1IF = 0; //clear flag
     INTCON2bits.INTEDG1 = 1; //rising edge
     INTCON3bits.INT1IP = 1; //high priority

     //setup timer interrupts

     //enable high level interrupts
     INTCONbits.GIE = 1;

     //enable low level interrupts
     //INTCONbits.GIEL = 1;

     //loop forever
     while(1)
     {

     }
}


//////////////////THIS IS OUR INTERRUPT SERVICE ROUTINE/////////////////////////
/* SOME THINGS TO KEEP IN MIND :
    Comments on the same line as the pragma statements here can cause problems*/
#pragma interrupt isr
void isr(void)
{    
    INTCONbits.GIE = 0; //disable interrupts
    if (INTCONbits.INT0IE&&INTCONbits.INT0IF) //Horn interrupt detected and turned on
    {
        if(INTCON2bits.INTEDG0) //if rising edge interrupt
        {
            Horn(ON); //turn horn on
            INTCON2bits.INTEDG0 = 0; // set to falling edge interrupt
        }
        else   //if falling edge interrupt
        {
            Horn(OFF); //turn horn off
            INTCON2bits.INTEDG0 = 1; //set to rising edge interrupt
        }
        INTCONbits.INT0IF=0; //clear interrupt flag
    }
    else if (INTCON3bits.INT1IE&&INTCON3bits.INT1IF) //Brake interrupt detected and turned on
    {
        if(INTCON2bits.INTEDG1) //if rising edge interrupt
        {
            Brake(ON); //turn on brake lights
            INTCON2bits.INTEDG1 = 0; //set to falling edge interrupt
        }
        else   //if falling edge interrupt
        {
            Brake(OFF); //turn off brake lights
            INTCON2bits.INTEDG1 = 1; //set to rising edge
        }
    }
    else if (0)
    {
        //check next flag
    }
    else
    {
        //check last flag
    }
    INTCONbits.GIE = 1; //enable interrupts
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

