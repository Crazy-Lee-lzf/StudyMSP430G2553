#include <msp430.h>
#include "OLED.h"
#include "Typedef.h"
#include "Other_Font.h"

const char cy[16] = "成语不全      ";
const char my[16] = "名人名言      ";
const char xs[16] = "看看小说      ";

extern char *OLED_Buf[4];

void Menu_Init(void){
    P2SEL &= ~0x0F;
    P2SEL2 &= ~0x0F;
    P2DIR &= ~0x0F;
    P2REN |= 0x0F;
    P2OUT |= 0x0F;
    P2IE |= 0x0F;
    P2IES |=0x0F;
    P2IFG &= ~0x0F;
}

void Menu_main(void){
    OLED_Buf[0] = cy;
    OLED_Buf[1] = my;
    OLED_Buf[2] = xs;
    OLED_Buf[3] = "李至峰  郭文豪";
}

void Menu_cy(uint8_t page){
    uint8_t i, s = (page - 1) * 4;
    for(i = 0;i < 4;i++){
        OLED_Buf[i] = cyAll[s + i];
    }
}

void Menu_my(uint8_t page){
    uint8_t i, s = (page - 1) * 4;
    for(i = 0;i < 4;i++){
        OLED_Buf[i] = myAll[s + i];
    }
}

void Menu_xs(uint8_t page){
    OLED_ShowFull(dpcq + (page - 1) * 64);
}


