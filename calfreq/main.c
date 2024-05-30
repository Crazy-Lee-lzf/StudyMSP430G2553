#include <msp430.h> 

void Timer_Init(){
   TA0CCTL0 = CCIE;
   TACCR0 = 1000 - 1;
   TACTL = TASSEL_2 | ID_0  | MC_1 | TACLR;

    TA1CCTL1 = CM_1 | CCIS_0 | CAP |CCIE;
    TA1CTL = TASSEL_2 | ID_0  | MC_2 | TACLR | TAIE;
}

void Init(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P2SEL |= 0x02;
    P2SEL2 &= ~0x02;
    P2DIR &= ~0x02; // PXDIR 0 ÎªÊäÈë

    Timer_Init();

    __enable_interrupt();
}

long Tcnt1 = 0, Tcnt2, icnt = 0, frq = 0;
long all = 0, high = 0, pre = 0;
long cntx = 0;
int main(void)
{
	Init();
    while(1){
        ;
    }

}
int x = 0;
int Edge = 1;

#pragma vector = TIMER1_A1_VECTOR
__interrupt void TimerA1_Isr(void){
    switch(TA1IV){
        case 0x02:
            if(Edge == 1){
                icnt ++;
                Edge = 0;
                TA1CCTL1 = CM_2 | CCIS_0 | CAP |CCIE;
                TA1CCTL1 &= ~CCIFG;

                all = Tcnt2 * 65536 + TA1CCR1;
                pre = 100l * high / all;
                Tcnt2 = 0;
            }
            else{
                TA1CCTL1 = CM_1 | CCIS_0 | CAP |CCIE;
                Edge = 1;
                high = Tcnt2 * 65536 + TA1CCR1;
            }
            break;
        case 0x04:
            break;
        case 0x0A:
            Tcnt2 ++;
            TA1CTL &= ~TAIFG;
            break;
        default:
            break;

    }
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
    Tcnt1++;
    if(Tcnt1 >= 1000){
        Tcnt1 = 0;
        frq = icnt;
        icnt = 0;
    }
}
