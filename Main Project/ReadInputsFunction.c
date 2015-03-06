#include "ReadInputsFunction.h"
#include <p18cxxx.h>

unsigned char InputRegister;
unsigned char ReadInputs(void)
 {
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
     //check BRK_SWITCH
     if(PORTBbits.RB1 == 1)
     {
         InputRegister |= (1 << BRK_SWITCH);
     }
     else
     {
         InputRegister &= ~(1 << BRK_SWITCH);
     }

     
     return(InputRegister);
 }

