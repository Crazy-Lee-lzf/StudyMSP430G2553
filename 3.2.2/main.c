#include <msp430.h>

#define Delay_ms(x) __delay_cycles((long)x * 1000)

unsigned char SMG[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
unsigned char str1[10] = {9, 2, 2, 0, 1, 5, 1, 8, 8, 8};
unsigned char str2[12] = {11, 1, 8, 2, 7, 4, 8, 4, 7, 6, 5, 0};
unsigned char str3[9] = {8, 2, 0, 2, 4, 0, 3, 2, 8};
unsigned short flag;
unsigned short cnt;

void Init(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1SEL = 0x00;
    P1SEL2 = 0x00;
    P1DIR = 0xFF;
    P1OUT = 0xFF;

    P2SEL &= ~0x01;
    P2SEL2 &= ~0x01;
    P2DIR &= ~0x01;
    P2REN |= 0x01;
    P2OUT |= 0x01;
    P2IE |= 0x01;
    P2IES |=0x01;
    P2IFG &= ~0x01;
    __enable_interrupt();
}

int main(void)
{
    flag = 0;
    cnt = 0;
    Init();
	while(1){
	    cnt ++;
	    if(flag == 0){
	        if(cnt > str1[0]) cnt = 1;
	        P1OUT = SMG[str1[cnt]];
	        Delay_ms(500);
	    }
	    else if(flag == 1){
            if(cnt > str2[0]) cnt = 1;
            P1OUT = SMG[str2[cnt]];
            Delay_ms(800);
        }
	    else if(flag == 2){
            if(cnt > str3[0]) cnt = 1;
            P1OUT = SMG[str3[cnt]];
            Delay_ms(1000);
        }
	}
	//return 0;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x01){
        Delay_ms(10);
        if(!(P2IN & 0x01)){
            flag ++;
            cnt = 0;
        }
        P2IFG &= ~0x01;
    }
}
