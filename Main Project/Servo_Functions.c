#include "Servo_Functions.h"
#include <p18cxxx.h>
#include <delays.h>  

void PWMSetup(char PWMInput)
{
    //see section 15.4 in pic18f2685 datasheet
    //this also helped
    //http://www.microchip.com/forums/m158343.aspx


    //Equation 15-1
    //PWM Period = [PR2 + 1] * 4 * TOSC * (TMR2 prescale)
    //Desired PWM frequency is 488.28125Hz (lowest possible with 8MHz clock)
    //488.28125 might be to fast for servo...
    //Fosc = 8MHz
    //TMR2 prescale = 16
    //1/488.28125=[PR2+1]*4*1/(8*10^6)*16
    //PR2 = 255 (highest value possible >> lowest frequency possible)


    //Find max resolution of the duty cycle the can be
    //used with a 488.28125Hz frequency
    //EQUATION 15-3
    //PWM Resolution (max) = log(FOSC/FPWM) / log(2) bits
        //1/488.28125=2^(PWM RESOLUTION) * 1/(8*10^6)*16
    //MAX PWM RESOLUTION = 10

    //Equation 15-2
    //PWM Duty Cycle = (CCPR1L:CCP1CON<5:4>)*TOSC*(TMR2 Prescale)
    //this is in [Time_high]/Tpwm, not percent
    //for example: 50% duty cycle
    //0.5 * (1/488.28125) = (CCPR1L:CCP1CON<5:4>)*(1/8*10^6)*16
    //(CCPR1L:CCP1CON<5:4>) = 512 = 0.5*1024
    //only really need 8 bit resolution
    //CCPR1L = (CCPR1L:CCP1CON<5:4>) shifted right 2 bits or divided by 4
    //CCPR1L = 128 = 0.5*256

    //15.4.4 SETUP FOR PWM OPERATION
    //set PWM period by writing to PR2 register
    PR2 = 255;
    //set PWM duty cycle by writing to the CCPR1L register and CCP1CON<5:4> bits
    CCPR1L = PWMInput;
    //Make CCP1 an output by clearing appropriate TRIS bit
    TRISCbits.TRISC2 = 0;
    //Set TMR2 prescale value then enable Timer 2 by writing to T2CON
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.TMR2ON = 1;
    //Configure CCP1 module for PWM operation
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;  //PWM mode

}


//updates duty cycle: 0 to 255
void PWMUpdate(char PWMInput)
{
    CCPR1L = PWMInput;
}

//Position wiper to a certain degree
void PositionWiper(int Degrees)
{
    //convert Degrees to a number 0 to 255
    //divide by MAX_DEGREES and multiply by SERVO_RANGE and add REST_POSITION
    Degrees = Degrees * SERVO_RANGE / MAX_DEGREES + REST_POSITION;
    PWMUpdate(Degrees); //might need to convert to char
}

void sweep(void) //sweeps servo back and forth once
{
    PWMUpdate(REST_POSITION);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    PWMUpdate(EXTREME_POSITION);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    Delay10KTCYx(10);
    PWMUpdate(REST_POSITION);
}
