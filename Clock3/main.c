#include <msp430.h>
#include "Timer.h"
#include "Shumaguan.h"
#include "Delay.h"


unsigned int time[6]={2024,4,11,16,36,0};
unsigned int Month[13]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
unsigned short alarm_time[2]={7,30};
unsigned short limit[6]={10000,12,31,24,60,60};
unsigned short time_flag = 1;
unsigned short show_flag = 1;
unsigned short edit_flag = 0;

void Init(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    SMG_Init();
    Timer_Init();
    __enable_interrupt();
    P2SEL &= ~0x0F;
    P2SEL2 &= ~0x0F;
    P2DIR &= ~0x0F;
    P2REN |= 0x0F;
    P2OUT |= 0x0F;
    P2IE |= 0x0F;
    P2IES |=0x0F;
    P2IFG &= ~0x0F;

    P2SEL &= ~0x80;
    P2SEL &= ~0x80;
    P2DIR |= 0x80;
    P2OUT &= ~0x80;
}

int main(void)
{
    Init();
    while(1){
        unsigned short temp_data[8] = {time[0]/10%10, time[0]%10, 16, time[1]/10, time[1]%10, 16, time[2]/10, time[2]%10};
        unsigned short temp_time[8] = {time[3]/10, time[3]%10, 16, time[4]/10, time[4]%10, 16, time[5]/10, time[5]%10};
        unsigned short temp_alarm[8] = {10, 12, 16, alarm_time[0]/10, alarm_time[0]%10, 16,alarm_time[1]/10,alarm_time[1]%10};
        unsigned short *ptemp;
        unsigned shan = 0;
        if(edit_flag > 0) shan = 0x03 << (edit_flag*3  - 3);
        if(show_flag == 0)  ptemp = temp_data;
        else if(show_flag == 1) ptemp = temp_time;
        else if(show_flag == 2){
            ptemp = temp_alarm;
            shan =shan << 3;
        }
        ShowTime(ptemp, shan);
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
    if(time[0] % 400 == 0||(time[0] % 4 == 0 && time[0] % 100 != 0)) Month[2] = 29;
    else Month[2] = 28;
    limit[2] = Month[time[1]];
    time_flag ++;
    if(time_flag == 100){
        if(time[3] == alarm_time[0] && time[4] == alarm_time[1])    P2OUT |= 0x80;
        else P2OUT &= ~0x80;
        time[5]++;
        time_flag = 0;
        if(time[5] >= 60){
            time[5] = 0;
            time[4]++;
            if(time[4] >= 60){
                time[4] = 0;
                time[3] ++;
                if(time[3] >= 24){
                    time[3] = 0;
                    time[2] ++;
                    if(time[2] > Month[time[1]]){
                        time[2] = 0;
                        time[1]++;
                        if(time[1] > 12){
                            time[0]++;
                        }
                    }
                }
            }
        }

    }
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x01){
        Delay_ms(10);
        if(!(P2IN & 0x01)){
            show_flag ++;
            if(show_flag > 2) show_flag = 0;
            edit_flag = 0;
        }
        P2IFG &= ~0x01;
    }
    if(P2IFG & 0x02){
        Delay_ms(10);
        if(!(P2IN & 0x02)){
            edit_flag ++;
            if(show_flag != 2 && edit_flag > 3) edit_flag = 0;
            else if(show_flag == 2 && edit_flag > 2) edit_flag = 0;
        }
        P2IFG &= ~0x02;
    }
    if(P2IFG & 0x04){
        Delay_ms(10);
        if(!(P2IN & 0x04)){
            if(edit_flag > 0){
                if(show_flag == 0){
                    time[edit_flag - 1]++;
                    if(time[edit_flag - 1] > limit[edit_flag - 1]) time[edit_flag - 1] = 1;
                }
                else if(show_flag == 1){
                    time[edit_flag - 1 + 3] ++;
                    if(time[edit_flag - 1 + 3] >= limit[edit_flag - 1 + 3]) time[edit_flag - 1 + 3] = 0;
                }
                else if(show_flag == 2){
                    alarm_time[edit_flag - 1] ++;
                    if(alarm_time[edit_flag - 1] >= limit[edit_flag - 1 + 3]) alarm_time[edit_flag - 1] = 0;
                }
            }
        }
        P2IFG &= ~0x04;
    }
    if(P2IFG & 0x08){
        Delay_ms(10);
        if(!(P2IN & 0x08)){
            if(edit_flag > 0){
                if(edit_flag > 0){
                    if(show_flag == 0){
                        time[edit_flag - 1]--;
                        if(time[edit_flag - 1] == 0) time[edit_flag - 1] = limit[edit_flag - 1];
                    }
                    else if(show_flag == 1){
                        if(time[edit_flag - 1 + 3] == 0) time[edit_flag - 1 + 3] = limit[edit_flag -1 + 3];
                        time[edit_flag - 1 + 3] --;
                    }
                    else if(show_flag == 2){
                        if(alarm_time[edit_flag - 1] == 0) alarm_time[edit_flag - 1] = limit[edit_flag -1 + 3];
                        alarm_time[edit_flag - 1] --;
                    }
                }
            }
        }
        P2IFG &= ~0x08;
    }
}
