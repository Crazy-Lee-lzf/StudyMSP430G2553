#include <msp430.h>

void Delay(int time){
    int n;
    while(time--){
        n=100;
        while(n--);
    }
}

int GetKey(void){
    int key = 0;
    if((P2IN & 0x01) == 0){
        Delay(10);
        if((P2IN & 0x01) == 0){
            while((P2IN & 0x01) == 0) ;
            key = 1;
        }
    }
    else if((P2IN & 0x02) == 0){
        Delay(10);
        if((P2IN & 0x02) == 0){
            while((P2IN & 0x02) == 0) ;
            key = 2;
        }
    }
    return key;
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

    int cnt = 0, key = 0, temp = 0;
    while(1){
        key = GetKey();
        if(key == 1){
            cnt = (cnt - 1 + 8) % 8;
        }
        else if(key == 2){
            cnt = (cnt + 1) % 8;
        }
        temp = 1 << cnt;
        P1OUT = temp;
    }
}
