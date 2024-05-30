#include <msp430.h> 


/**
 * main.c
 */
unsigned char SMG[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x40,0x40};

int cnt = 0;
int showflag = 0;
int timecnt = 0;
unsigned long int hour,minute,second;
unsigned long int nowt,data;

#define Delay_us(x) __delay_cycles((long)x);
#define Delay_ms(x) __delay_cycles((long)x * 1000)

void SMG_Init(){
    P1SEL = 0x00;
    P1SEL2 = 0x00;
    P1DIR = 0xFF;
    P1OUT = 0xFF;

    P3SEL = 0x00;
    P3SEL = 0x00;
    P3DIR = 0xFF;
    P3OUT = 0xFF;
}

void Timer_Init(void){
    TA0CCTL0 = CCIE;
    TACCR0 = 2000 - 1;
    TACTL = TASSEL_2 | MC_1 | ID_0 | TACLR;  // 0.002s
    __enable_interrupt();
}

void Key_Init(){
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

void Init(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    SMG_Init();
    Timer_Init();
    Key_Init();
    nowt = 0x04f05f00;
    data = 0x20240418;
}
inline void addh(){
    nowt += 0x01000000;
    if((nowt & 0x0F000000) == 0x0a000000){
        nowt += 0x10000000;
        nowt &= ~0x0F000000;
    }
    if(((nowt & 0x0F000000) == 0x04000000) && ((nowt & 0xF0000000) == 0x20000000)){
        //addday();
    }
}

inline void addm(){
    nowt += 0x00001000;
    if((nowt & 0x0000F000) == 0x0000A000){
        nowt += 0x00010000;
        nowt &= ~0x0000F000;
    }
    if((nowt & 0x000FF000) == 0x00060000){
        nowt &= ~0x000F0000;
        addh();
    }
}

inline void adds(){
    second ++;
    if((second & 0x0f) == 0x0a){
        second += 0x10;
        second &= ~0x0f;
    }
    else if()
    nowt = (1ll * (hour & 0xFF) << 24ll) | (1ll * (minute & 0xFF) << 12ll) | (second & 0xFF) |(1ll * 0xf << 8) | (1ll * 0xf << 20);
}

int main(void)
{
    Init();
	showflag = 1;
    while(1){
        ;
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
    P1OUT = 0xFF;
    P3OUT = ~(1 << cnt);
    if(showflag == 0) P1OUT = SMG[(data >> (cnt << 2)) & 0x0F];
    else P1OUT = SMG[(nowt >> (cnt << 2)) & 0x0F];
    cnt ++;
    if(cnt >= 8) cnt = 0;
    timecnt++;
    if(timecnt >=100){
        adds();
        timecnt = 0;
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x01){
        Delay_ms(10);
        if(!(P2IN & 0x01)){
            showflag ^= 0x01;
        }
        P2IFG &= ~0x01;
    }
}
