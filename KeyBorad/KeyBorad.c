/*
 * KeyBorad.c
 *
 *  Created on: 2024��3��9��
 *      Author: Administrator
 */
#include <msp430.h>
void Delay(int time){
    int n;
    while(time--){
        n = 100;
        while(n--);
    }
}

void KeyBoradInit(void){
    //�У�OUTPUT
    P3DIR |= 0x0F;
    P3OUT |= 0x0F;

    //�У�INPUT
    P3DIR &= ~0xF0;
    P3REN |= 0xF0;
    P3OUT |= 0xF0;
}

int GetKeyBoradValue(void){
    int KeyValue = 0, i, j, temp;
    for(i = 0;i < 4;i++){
        temp = 0x01 << i;
        P3OUT &= ~temp;
        for(j = 0;j < 4;j++){
            temp = (1 << (j+4) );
            if( (P3IN & temp) == 0 ){
                Delay(10);
                if( (P3IN & temp) == 0 ){
                    while( (P3IN & temp ) == 0 );
                    KeyValue = i * 4 + j + 1;
                }
            }
        }
        temp = 0x01 << i;
        P3OUT |= temp;
    }
    return KeyValue;
}

