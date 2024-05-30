#include <msp430.h>

#define Delay_us(x) __delay_cycles((long)x)
#define Delay_ms(x) __delay_cycles((long)x * 1000)

unsigned short flag = 0;

void Init(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1SEL &= ~0x03;
    P1SEL2 &= ~0x03;
    P1DIR |= 0x03;
    P1OUT |= 0x03;

    P2SEL &= 0x03;
    P2SEL2 &= 0x03;
    P2DIR &= ~0x03;
    P2REN |= 0x03;
    P2OUT |= 0x03;
    P2IE |= 0x03;
    P2IES |= 0x03;
    P2IFG &= ~0x03;
    __enable_interrupt();
}

int main(void)
{
	Init();

	while(1){
	   if(flag == 0) Delay_ms(200);
	   else if(flag == 1) Delay_ms(1000);
	   else if(flag == 2) Delay_ms(4000);
	   P1OUT ^= 0x03;
	}
	//return 0;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x01){
        Delay_ms(10);
        if(!(P2IN & 0x01)){
            flag++;
            if(flag >=3) flag = 0;
            P2IFG &= ~0x01;
        }
    }
    if(P2IFG & 0x02){
        Delay_ms(10);
        if(!(P2IN & 0x02)){
            P1OUT ^= 0x01;
            P2IFG &= ~0x02;
        }
    }
}
