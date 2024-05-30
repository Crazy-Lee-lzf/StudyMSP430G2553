#include <msp430.h>
#include "Delay.h"
unsigned char SMG[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,0x40};
unsigned short cnt_shan;

void SMG_Init(){
    P1SEL = 0x00;
    P1SEL2 = 0x00;
    P1DIR = 0xFF;
    P1OUT = 0xFF;

    P3SEL = ~0x07;
    P3SEL = ~0x07;
    P3DIR = 0x07;
    P3OUT = ~0x07;

    cnt_shan = 0;
}


void Display(unsigned short wei, unsigned short duan){
    P3OUT = wei;
    P1OUT = SMG[duan];
}


void ShowTime(unsigned short *temp,unsigned short shan){
    int i;
    cnt_shan ++;
    for(i = 0;i < 8;i ++){
        if(shan & (1 << i )){
            if(cnt_shan <= 30) continue;
            else if(cnt_shan >= 60) cnt_shan = 0;
        }
        Display(i, *(temp + i));
        Delay_ms(2);
    }
}
