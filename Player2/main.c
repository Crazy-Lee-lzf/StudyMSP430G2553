#include <msp430.h>
#include "OLED.h"
#include "MySPI.h"
#include "Timer.h"
#include "W25Q64.h"
#include "MCP4075.h"
#include "Typedef.h"
#include "OLED_Font.h"

void Init(){
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
   // OLED_Init();
    W25Q64_Init();
    MCP4075_Init();
    Timer_Init();
    P3SEL &= ~0x01;
    P3SEL2 &= ~0x01;
    P3DIR |= 0x01;
    P3OUT |= 0x01;
}

uint8_t data[256];
uint16_t part, note;
uint8_t n = 71, flag = 0;
uint32_t add;
uint16_t i, j, cnt = 0;

int main(void) {
    Init();
    add = 0x020000;
    while(1){
        P3OUT &= ~0x01;
        for(i = 0;i < 4095;i++){
            MCP4075_WriteData(i);
        }
        P3OUT |= 0x01;
        for(i = 0;i < 4095;i++){
            MCP4075_WriteData(i);
        }
    }


}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
        MCP4075_WriteData(data[(part << 6) + note]);
		note ++;
		cnt ++;
		if(note >= 64){
			note = 0;
			part ++;
			if(part >= 4) part = 0;
			flag = 0;
		}
}

