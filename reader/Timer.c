#include <msp430.h>

void Timer_Init(void){
    TA0CCTL0 = CCIE;
    TACCR0 = 10000;
    TACTL = TASSEL_2 | ID_0  | MC_1 | TACLR;
    __enable_interrupt();
}
