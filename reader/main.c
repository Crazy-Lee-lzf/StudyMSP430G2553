#include <msp430.h>
#include "OLED.h"
#include "Menu.h"
#include "MySPI.h"
#include "Timer.h"
#include "W25Q64.h"
#include "Typedef.h"

#define Delay_us(x) __delay_cycles((long)x);
#define Delay_ms(x) __delay_cycles((long)x * 1000)

void Init(){
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    Menu_Init();
    OLED_Init();
    Timer_Init();
    W25Q64_Init();

    __enable_interrupt();
}

char *OLED_Buf[4];
int gun_cnt = -0x01;
int z = 0;

uint8_t mode = 1;
uint8_t chosed = 1;
uint8_t menu = 0;
uint8_t page = 1;
uint8_t page_xs = 1;
uint8_t page_limit[] = {1, 2, 2, 1};
uint8_t last_page[] = {4, 1, 1, 1};
uint16_t time_cnt = 0;

void reload(void){
    gun_cnt = -0x01;
    if(mode == 2){
        Menu_xs(page_xs);
    }
    if(menu == 0){
        Menu_main();
    }
    else if(menu == 1){
        Menu_cy(page);
    }
    else if(menu == 2){
        Menu_my(page);
    }

}

int main(void) {
    Init();
    OLED_Clear();
    reload();

    //Menu_my(1);
    while(1){
        ;
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
    if(mode == 2) return;
    time_cnt ++;
    if(time_cnt < 50) return;
    time_cnt = 0;
    gun_cnt++;

    uint8_t i = 0;
    for(i = 0;i < 4;i++){
        if(chosed == i + 1) OLED_Show_ChLine(i + 1, OLED_Buf[i], 1, &gun_cnt);
        else OLED_Show_ChLine(i + 1, OLED_Buf[i], 0, &z);
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x01){
        Delay_ms(10);
        if(!(P2IN & 0x01)){
            if(mode == 2){
                if(page_xs > 1)  page_xs --;
                reload();
                return ;
            }
            chosed --;
            gun_cnt = -1;
            if(chosed < 1){
                page --;
                chosed = 4;
                if(page < 1){
                    page = page_limit[menu];
                    chosed = last_page[menu];
                }
                reload();
            }
        }
        P2IFG &= ~0x01;
    }
    else if(P2IFG & 0x02){
        Delay_ms(10);
        if(!(P2IN & 0x02)){
            if(mode == 2){
                page_xs ++;
                reload();
                return ;
            }
            chosed ++;
            gun_cnt = -1;
            if(chosed > 4 || (page == page_limit[menu] && chosed > last_page[menu])){
                chosed = 1;
                page ++;
                if(page > page_limit[menu]){
                    page = 1;
                    chosed = 1;
                }
                reload();
            }
        }
        P2IFG &= ~0x02;
    }
    else if(P2IFG & 0x04){
        Delay_ms(10);
        if(!(P2IN & 0x04)){
            if(menu == 0){
                if(chosed == 1){
                    menu = 1;
                    mode = 1;
                    reload();
                }
                else if(chosed == 2){
                    menu = 2;
                    chosed = 1;
                    mode = 1;
                    reload();
                }
                else if(chosed == 3){
                    menu = 3;
                    mode = 2;
                    reload();
                }
            }
        }
        P2IFG &= ~0x04;
    }
    else if(P2IFG & 0x08){
        Delay_ms(10);
        if(!(P2IN & 0x08)){
            if(menu != 0){
                chosed = menu;
                menu = 0;
                mode = 1;
            }
            reload();
        }
        P2IFG &= ~0x08;
    }
}
