#include <p18cxxx.h>
#include <SetUpTimer_Function.h>

//returns system clock rate in KHz
unsigned int FindSystemClock()
{
    //read registers to figure out system clock rate
    if(OSCCON&0x80)//if IRCF2 = 1;
    {
        if (OSCCON&0x20)//if IRCF1 = 1;
        {
            if (OSCCON&0x10)//if IRCF0 = 1;
            {
                //clock is 8MHz
                //check PLL
                if(OSCTUNE&0x80)
                {
                    //PLL is enabled
                    return(32000);
                }
                else
                {
                    //PLL disabled
                    return(8000);
                }
            }
            else //IRCF0 = 0
            {
                //clock is 4MHz
                //check PLL
                if(OSCTUNE&0x80)
                {
                    //PLL in enabled
                    return(16000);
                }
                else
                {
                    //PLL is disabled
                    return(4000);
                }
            }
        }
        else //IRCF1 = 0
        {
            if (OSCCON&0x10)//if IRCF0 = 1
            {
                //Clock is 2 MHz
                return(2000);
            }
            else //IRCF0 = 0
            {
                //Clock is 1 MHz
                return(1000);
            }
        }
    }
    else // IRCF2 = 0;
    {
        if (OSCCON&0x20)//if IRCF1 = 1;
        {
            if (OSCCON&0x10)//if IRCF0 = 1;
            {
                //clock is 500 KHz
                return(500);
            }
            else //IRCF0 = 0
            {
                //clock is 250 KHz
                return(250);
            }
        }
        else //IRCF1 = 0
        {
            if (OSCCON&0x10)//if IRCF0 = 1
            {
                //Clock is 125 KHz
                return(125);
            }
            else //IRCF0 = 0
            {
                //Clock is 31 KHz
                return(31);
            }
        }
    }
}

//sets up timer0 to create a interrupt on a certain time interval
//requires low level interrupts to be enabled
//requires a low level isr
void SetUpTimer(void)
{    
    //calculate a prescaler for timer0
    //setup prescaler
    //setup interrupts
    //start timer
}
