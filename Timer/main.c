#include <msp430.h> 
#include "Timer.h"

/**
 * main.c
 */
void Delay(int time){
    int n = 100;
    while(time--){
        n = 100;
        while(n--);
    }
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Timer_Init();
	int x=0;
	while(1){
	    TA0CCR1 = 100*x;
	    x = (x+1)%101;
	    Delay(10);
	}
}
/*
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
    P2OUT ^=0x01;
}
*/
