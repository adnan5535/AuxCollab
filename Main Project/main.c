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
#include <stdio.h>
#include <stdlib.h>
//add these to project as existing files if it won't compile
#include "ReadInputsFunction.h"
#include "SetIO_Function.h"
#include "Servo_Functions.h"

#pragma config OSC = IRCIO67    // Oscillator Selection Bit: IRCIO67 isn't anywhere in datasheet...???...???
#pragma config BOREN = OFF      // Brown-out Reset disabled in hardware and software
#pragma config WDT = OFF        // Watchdog Timer disabled (control is placed on the SWDTEN bit)

/*
 * 
 */

unsigned int time1 = 0;
unsigned int time2 = 0;
unsigned int time3 = 0;
unsigned int millis = 0; //keeps track of time.
//min/max value depends on timer0 config
//look at low isr

#define ON 1
#define OFF 0

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

//wiper variables
int ttarget;
int LeftEdge;
int RightEdge;
int CurrentTime;
int PrevServoTime;
int ServoPeriod;
int aangle;

char x = 0;
int flag=0;

void main ()
 {
     InitEcoCar(); //disables a bunch of stuff
     //also sets clock rate
     //I changed from 32 to 8 MHz    
     SetIO(); //set up inputs and outputs     
     ReadInputs();
     PWMSetup(REST_POSITION);

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

     //setup timer0 interrupts for millis function
     //or some sort of timer function centiseconds, deciseconds, etc...
     T0CONbits.T08BIT = 0; //16 bit timer/counter
     T0CONbits.T0CS = 0; //internal instruction cycle clock
     //T0CONbits.T0SE  //edge select
     T0CONbits.PSA = 0; //prescaler is assigned
     T0CONbits.T0PS2 = 0; //8 prescaler
     T0CONbits.T0PS1 = 0;
     T0CONbits.T0PS0 = 1;
     //timer overflow interval is (1/(FOSC))*4*prescaler*(2^16-1)??
     //use that value in low isr
     T0CONbits.TMR0ON = 1; //enable
     INTCONbits.TMR0IE = 1; //timer 1 overflow interrupt enable
     INTCONbits.TMR0IF = 0; //clear flag
     INTCON2bits.TMR0IP = 0; //low priority

     //enable high level interrupts
     INTCONbits.GIE = 1;

     //enable low level interrupts
     INTCONbits.GIEL = 1;

     sweep(); //test wiper

     while(0)
     {
         if(((PORTCbits.RC4 == 1) || ((aangle!=LeftEdge)) && ((CurrentTime-PrevServoTime)>ServoPeriod)))
         {
             if (aangle==ttarget)
             {
                 if(ttarget==RightEdge)
                 {
                     ttarget=LeftEdge;
                 }
                 else
                 {
                         ttarget=RightEdge;
                 }
             }
 
             //LATCbits.LATC2 = 1;
         }
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



//////////////////THIS IS OUR LOW PRIORITY INTERRUPT SERVICE ROUTINE/////////////////////////
/* SOME THINGS TO KEEP IN MIND :
    Comments on the same line as the pragma statements here can cause problems*/
#pragma interrupt low_isr
void low_isr(void)
{
    if (INTCONbits.TMR0IE&&INTCONbits.TMR0IF) //if timer0 interrupt is enabled and triggered
    {
        INTCONbits.TMR0IF = 0; //clear flag
        //timer0 overflow interval is (1/(FOSC))*4*prescaler*(2^16-1)
        //currectly 1/(4*10^6)*4*4*(2^8-1) = 262.14ms
        millis = millis + 262; //add interval to millis

        //make a if statement run every second
        if ((millis - time1) > 1000)
        {
            time1 = millis;
            LATAbits.LATA1 ^= 1; //toggle error led (flashing lights cool)
            ReadInputs();
            //look at InputRegister
        }
    }
}

//////////////////THIS IS OUR INTERRUPT/////////////////////////
// This calls the interrupt service routine when the interrupt is called

#pragma code low_vector = 0x18

/* this 'function' can only be 8 bytes in length. This is why the code we want
 * to run with our interrupt will be put in the interrupt service routine   */
void low_interrupt(void){

    /* This is an assembly instruction. This efficiently calls our interrupt
     * service routine */
    _asm GOTO low_isr _endasm
}

/* There is actually a space at the end of this line and a line break after...
 * This is important for some strange reason */
#pragma code
