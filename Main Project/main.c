/* 
 * File:   main.c
 * Author: Adnan and Reegan
 *
 * Created on January 26, 2015, 6:28 PM
 */
//written for C18 compiler
#include <p18cxxx.h> 
#include "J1939.h" 
#include "ecocar.h"
#include <delays.h>  
#include <stdio.h>
#include <stdlib.h>


#pragma config OSC = IRCIO67    // Oscillator Selection Bit: IRCIO67 isn't anywhere in datasheet...???...???
#pragma config BOREN = OFF      // Brown-out Reset disabled in hardware and software
#pragma config WDT = OFF        // Watchdog Timer disabled (control is placed on the SWDTEN bit)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

/*
 * 
 */

#include "ReadInputsFunction.h"
#include "SetIO_Function.h"
#include "Servo_Functions.h"
#include "LightOutput_Functions.h" //this file has a definition for OFF = 0 which messes with config statements above ^^^^
                                   //fix by putting after config statements
                                   //would probably be better to just to change ON/OFF to HIGH/LOW


//To different loops in main function one for normal opperation and one might be useful for debugging
#define TEST_MODE 7
#define RUN_MODE 6
unsigned char mode = RUN_MODE; //select mode here


unsigned int Time1 = 0;
unsigned int Time2 = 0;
unsigned int Time3 = 0;

#define UP -1
#define DOWN 1
unsigned int ReadTimeInterval = 250;  //the interval that we read inputs at
unsigned int MainTimeInterval = 500; //the interval that everything flashes at
unsigned int ServoTimeInterval = 50; //the interval the servo switches at
unsigned int WiperDirection = 0;


unsigned char ToggleByte = 0; //for toggling lights on or off every MainTimeInterval

unsigned char ServoTimingByte = 0;
unsigned char ServoPosition = 0; //change this to output register

unsigned int millis = 0; //keeps track of time.
//min value depends on timer0 config
//look at low isr



void main ()
 {
     InitEcoCar(); //disables a bunch of stuff (ADC)
     //also sets clock rate
     //I changed from 32 to 4 MHz (I think)
     SetIO(); //set up inputs and outputs   
     ReadInputs(); //update input register
     PWMSetup(REST_POSITION); // Configure PWM and Position servo
     ServoPosition = REST_POSITION;
     //enable interrupt priorities
     RCONbits.IPEN = 1;

     //set up int0
//     INTCONbits.INT0IE = 1; //enable int0
     INTCONbits.INT0IF=0; //set flag to 0
     INTCON2bits.INTEDG0 = 1; //set to rising edge
     //is always high priority

     //setup int1
//     INTCON3bits.INT1IE = 1; //enable
     INTCON3bits.INT1IF = 0; //clear flag
     INTCON2bits.INTEDG1 = 1; //rising edge
     INTCON3bits.INT1IP = 1; //high priority

     //setup timer0 interrupts for keeping track of time (millis);
     T0CONbits.T08BIT = 0; //16 bit timer/counter
     T0CONbits.T0CS = 0; //internal instruction cycle clock
     //T0CONbits.T0SE  //edge select
     T0CONbits.PSA = 1; //prescaler is not assigned
     T0CONbits.T0PS2 = 0; //2 prescaler
     T0CONbits.T0PS1 = 0;
     T0CONbits.T0PS0 = 0;
     //timer overflow interval is (1/(FOSC))*4*prescaler*(2^16-1)??
     //use that value in low isr
     T0CONbits.TMR0ON = 1; //enable
     INTCONbits.TMR0IE = 1; //timer 1 overflow interrupt enable
     INTCONbits.TMR0IF = 0; //clear flag
     INTCON2bits.TMR0IP = 0; //low priority

     //enable low level interrupts
     INTCONbits.GIEL = 1;

     //enable high level interrupts
     INTCONbits.GIE = 1;


     LATAbits.LATA1 = 0;

     while(millis < 750)
     {

     }




   while(mode == RUN_MODE) //actual code
   {
       ReadInputs();

        //compare time to time intervals
        if ((millis - Time1) > MainTimeInterval) //Signal and Haz lights flash at MainTimeInterval
        {
            Time1 = millis;
            ToggleByte ^= 0xff; //this byte toggles between 0 and all ones every MainTimeInterval
                            //can use to make everything flash on or off
        }


       if ((millis - Time3) > ServoTimeInterval)
       {
           Time3 = millis;
           ServoPosition = PWMUpdate(ServoPosition + WiperDirection);
           if (WiperDirection == 0)
           {
               PWMUpdate(0);
           }
       }

       if(ServoPosition < FAR_POSITION)
       {
           WiperDirection = DOWN;
       }
       if(ServoPosition > (REST_POSITION - 6))
       {
           if (ServoPosition != REST_POSITION )
           WiperDirection = UP;
       }
       if (InputRegister & (1 << WIPER_SWITCH))
       {
           if(WiperDirection == 0)
           {
               WiperDirection = UP;
           }
       }
       else
       {
           if (ServoPosition > (REST_POSITION - 3))
           {
               WiperDirection = 0;
               ServoPosition = PWMUpdate(REST_POSITION);

           }
           else
           {
               WiperDirection = DOWN;
           }

       }




       if (InputRegister & (1 << BRK_SWITCH))
       {
           Brake(ON);
       }

        if (InputRegister & (1 << HORN_SWITCH))
        {
            Horn(ON);
        }
        else
        {
            Horn(OFF);
        }



        Error(ToggleByte); //flash error led (flashing lights cool)


        //turn on/off signal/hazard lights
        if (InputRegister & (1 << BLINK_R_SWITCH))
        {
            Signal(RIGHT&ToggleByte);
        }
        else if (InputRegister & (1 << BLINK_L_SWITCH))
        {
            Signal(LEFT&ToggleByte);
        }
        else if (InputRegister & ( 1 << HAZ_SWITCH))
        {
            Haz(ON&ToggleByte);
        }
        else
        {
            Signal(OFF);
        }
   }
}


//////////////////THIS IS OUR INTERRUPT SERVICE ROUTINE/////////////////////////
/* SOME THINGS TO KEEP IN MIND :
    Comments on the same line as the pragma statements here can cause problems*/
#pragma interrupt isr
void isr(void) //keep as little code in isr as possible
{
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
void low_isr(void) //keep as little code in isr as possible
{
    //this interrupt runs every time timer0 overflows
    if (INTCONbits.TMR0IE&&INTCONbits.TMR0IF) //if timer0 interrupt is enabled and triggered
    {
        INTCONbits.TMR0IF = 0; //clear flag
        //timer0 overflow interval is (1/(FOSC))*4*prescaler*(2^16-1)
        //currectly 1/(4*10^6)*4*2*(2^8-1) = 131.07ms
        millis = millis + 67; //add interval to millis
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
