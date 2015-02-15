#include "SetIO_Function.h"
#include <p18cxxx.h>

void SetIO(void)
{
     //set up input/ouputs
     TRISBbits.TRISB0 = 1;//HORN_SWITCH input
     TRISAbits.TRISA0 = 0; //HORN_FET output

     TRISBbits.TRISB1 = 1; //BRAKE_SWITCH input
     TRISCbits.TRISC0 = 0; //BRK_L_HIGH output
     TRISCbits.TRISC1 = 0; //BRK_R_HIGH output

     //other inputs
     TRISCbits.TRISC4 = 1; //Wipers Switch input
     TRISCbits.TRISC5 = 1;
     TRISCbits.TRISC6 = 1;
     TRISCbits.TRISC7 = 1;

     //other outputs
     TRISCbits.TRISC2=0; //SERVO (For the Wipers)
}