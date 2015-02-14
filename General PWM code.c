//pwm code
//pic pwm module is too fast

//Set up PWM period by writing to the PR2 register
//PWM_period = 20ms
//Clock_Frequency = 4000000
//TMR2 Prescale Value = 16
//PR2_Value = PWM_period / (TMR2 Prescale Value) * Clock Frequency / 4 - 1
PR2 = 155;

//PWM duty cycle CCPR1L and CCP1CON<5:4>
//PR2 is 8 bits so only going to worry about 8 bit CCPRIL
//CCPR1L:CCP1CON<5:4> = (PWM DUTY Cycle) / (Clock Frequency) * (TMR2 Prescale Value)
// = PWM DUTY Cycle  
CCPR1L = 
//make CCP1 an output
TRISCbits.TRISC
//TMR2 prescale
//enable timer2
//Configure CCP1 for PWM