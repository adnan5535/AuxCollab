#include "ReadInputsFunction.h"
#include <p18cxxx.h>

char InputRegister;
//list of bits
#define WIPER_SWITCH 0
#define HAZ_SWITCH 1
#define HORN_SWITCH 2
#define BLINK_L_SWITCH 3
#define BLINK_R_SWITCH 4
//bits 5 , 6 and 7 unused


char ReadInputs(void)
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

