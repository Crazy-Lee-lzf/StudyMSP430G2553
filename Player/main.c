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
}

uint8_t data[256];
uint16_t part, note;
uint8_t n = 71, flag = 0;
uint32_t add;
uint16_t i, j, cnt = 0;

int main(void) {
    Init();
     while(1){
          for(i = 0;i < n;i++){
              for(j = 0;j < 512;j++){
                  add = 0x020000 +  i * 0x4000 + j * 32;
                  W25Q64_ReadData(add, data + (part << 5), 32);
                  flag = 1;
                  __enable_interrupt();
                  while(flag);
              }
          }
          MCP4075_WriteData(0);
          __disable_interrupt();
          while(1);
        }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
        MCP4075_WriteData(data[(part << 5) + note]);
		note ++;
		cnt ++;
		if(note >= 32){
			note = 0;
			part ++;
			if(part >= 8) part = 0;
			flag = 0;
		}
}

