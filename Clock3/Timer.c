#include <msp430.h>


void Timer_Init(void){
    TA0CCTL0 = CCIE;
    TACCR0 = 10537;
    TACTL = TASSEL_2 | MC_1 | ID_0 | TACLR;
    __enable_interrupt();
}

