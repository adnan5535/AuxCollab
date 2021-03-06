#include "LightOutput_Functions.h"
#include "ReadInputsFunction.h"
#include <p18cxxx.h>

unsigned char OutputRegister;


void Error(unsigned char ErrorInput) //turns error led on and off
{
    if (ErrorInput)
    {
        LATAbits.LATA1 = 1;
    }
    else
    {
        LATAbits.LATA1 = 0;
    }
}


void Haz(unsigned char HazInput)
{
    if (HazInput)
    {
        //turn lights on
        LATBbits.LATB4 = 1; //front left
        LATBbits.LATB5 = 1; //front right
        LATCbits.LATC0 = 1; //back left
        LATCbits.LATC1 = 1; //back right
        //update output register
        OutputRegister |= (1 << BLINK_L)|(1 << BLINK_R)|(1 << BRK_L_HIGH)|(1 << BRK_R_HIGH);
    }
    else
    {
        //turn lights off
        //LATBbits.LATB4 = 0; //front left
        //LATBbits.LATB5 = 0; //front right
        //LATCbits.LATC0 = 0; //back left
        //LATCbits.LATC1 = 0; //back right
        //update output register
        //OutputRegister &= ~((1 << BLINK_L)|(1 << BLINK_R)|(1 << BRK_L_HIGH)|(1 << BRK_R_HIGH));
        Signal(OFF); //Signal deals with this already, might as well save code
    }
}



void Horn(unsigned char HornInput) //turns horn on and off
{
    if (HornInput)
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

void Brake(unsigned char BrakeInput) //turns brake lights on and off
{
    if(BrakeInput)
    {
        if(InputRegister & (1 << BLINK_L_SWITCH))
        {
            //only Right right light on
            LATCbits.LATC0 = 1; //right
            LATCbits.LATC1 = 0;
        }
        else if(InputRegister & (1 << BLINK_R_SWITCH))
        {
            //only left light on
            LATCbits.LATC1 = 1;
            LATCbits.LATC0 = 0;
        }
        else
        {
            LATCbits.LATC0 = 1;
            LATCbits.LATC1 = 1;
        }
        OutputRegister |= (1 << BRK_L_HIGH) | (1 << BRK_R_HIGH);
    }
    else
    {
        LATCbits.LATC1 = 0; //left
        LATCbits.LATC0 = 0; //right
        OutputRegister &= ~((1 << BRK_L_HIGH) | (1 << BRK_R_HIGH));
    }
}


void Signal(unsigned char SignalInput)
{
    //first make sure both rear lights are on or off based on whether brake lights are on or off
    if (InputRegister&(1 << BRK_SWITCH))
    {
        Brake(ON);
    }
    else
    {
        Brake(OFF);
    }
    //make sure front signal lights are off
    LATBbits.LATB4 = 0; //left off
    LATBbits.LATB5 = 0; //right off

    //Set signal lights according to input
    if (SignalInput == LEFT)
    {
        //toggle Left Signal light
        LATBbits.LATB4 = 1; //front left on
        LATCbits.LATC1 = 1; //back left
        OutputRegister |= ( 1 << BLINK_L)|(1 << BRK_L_HIGH);
    }
    else if ( SignalInput == RIGHT )
    {
        //toggle Right Signal Lights
        LATBbits.LATB5 = 1; //Front right
        LATCbits.LATC0 = 1; //back right on
        OutputRegister |= (1 << BLINK_R)|(1 << BRK_R_HIGH);
    }
    else
    {
        //lights are already off
        OutputRegister &= ~((1 << BLINK_L)|(1 << BLINK_R)|(1 << BRK_L_HIGH)|(1 << BRK_R_HIGH));
    }
}
