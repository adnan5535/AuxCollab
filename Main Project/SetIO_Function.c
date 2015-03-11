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

     TRISAbits.TRISA1 = 0; //error led

     TRISCbits.TRISC4 = 1; //Wipers Switch input
     TRISCbits.TRISC2 = 0; //SERVO (For the Wipers)

     TRISCbits.TRISC5 = 1;//Hazards switch

     TRISCbits.TRISC6 = 1;//right signal switch
     TRISCbits.TRISC7 = 1;//left signal switch

     TRISBbits.TRISB4 = 0; //left signal light
     TRISBbits.TRISB5 = 0; //right signal light


     //other outputs

}