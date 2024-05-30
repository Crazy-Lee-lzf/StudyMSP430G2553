#include <msp430.h>

void Delay(int time){
    int n;
    while(time--){
        n=100;
        while(n--);
    }
}

void main(void){
    WDTCTL = WDTPW | WDTHOLD;
    //INPUT
    P2SEL &= ~0x03;
    P2SEL2 &= ~0x03;
    P2DIR &= ~0x03;
    P2REN |= 0x03;
    P2OUT |= 0x03;


    //OUTPUT
    P1DIR = 0xFF;

    int cnt = 0, k = 1;
    while(1){
    if((P2IN & 0x01) == 0){
            Delay(10);
            if((P2IN & 0x01) == 0){
                while((P2IN & 0x01) == 0) ;
                cnt = cnt + k;
                if(cnt >= 8) cnt = 0;
                else if(cnt < 0) cnt = 7;
            }
        }
        P1OUT = (1 << cnt);
        Delay(500);
    }
}
