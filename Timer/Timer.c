#include <msp430.h>


void Timer_Init(void){
    P1SEL |= 0xFF;
    P1DIR |= 0xFF;

    TA0CCTL0 = CCIE;
    TACCR0 = 10000;
    TA0CCTL1 = OUTMOD_7;
    TACTL = TASSEL_2 | MC_1;
}
