#include <msp430.h>



#define Delay_us(x) __delay_cycles((long)x);

#define Delay_ms(x) __delay_cycles((long)x * 1000)



unsigned char SMG[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};



// 定时器的初始化

void Timer_Init(){

    TA0CCTL0 = CCIE;

    TACCR0 = 10000 - 1;

    TACTL = TASSEL_2 | ID_0  | MC_1 | TACLR;  // 10ms  0.01s

}



void Init(){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer



    Timer_Init();



    P1SEL &= ~0x01;

    P1SEL2  &= ~0x01;

    P1DIR |= 0x01;

    P1OUT |= 0x01;



    //按键设置 这里是将P2.0 - P2.3配置为GPIO的输入 并启用上拉电阻

    P2SEL &= ~0x0F;

    P2SEL2 &= ~0x0F; //配置成GPIO口

    P2DIR &= ~0x0F; // PXDIR 0 为输入

    P2REN |= 0x0F; // PXREN 1  启用上下拉电阻

    P2OUT |= 0x0F;  // 输入模式下 PXOUT 1 启用上拉电阻

    P2IE |= 0x0F;   //启用中断

    P2IES |=0x0F;   //设置为为下降沿触发

    P2IFG &= ~0x0F; //清除中断标志位



    // 将P3.0 - P3.7 设置为输出

    P3SEL = 0x00;

    P3SEL2 = 0x00;

    P3DIR = 0xFF;

    P3OUT = 0xFF;

    __enable_interrupt(); //启用中断

}



int mode = 0;

int tcnt = 0;

int tcnt2 = 0;

int shan = 0;

int shan2 = 0;



int main(void)

{

    Init();

    P1OUT |= 0x01;

    while(1){
        P3OUT = ~SMG[mode + 1];
    }

}



#pragma vector = PORT2_VECTOR

__interrupt void PORT2_ISR(void){

    if(P2IFG & 0x01){

        Delay_ms(10);

        if(!(P2IN & 0x01)){

            while(!(P2IN & 0x01)) ;
            Delay_ms(10);

            mode = 0;

            tcnt = 0;

        }

        P2IFG &= ~0x01;

    }

    else if(P2IFG & 0x02){

        Delay_ms(10);

        if(!(P2IN & 0x02)){

            while(!(P2IN & 0x02)) ;
            Delay_ms(10);

            mode = 1;

            tcnt = 0;

        }

        P2IFG &= ~0x02;

    }

    else if(P2IFG & 0x04){

        Delay_ms(10);

        if(!(P2IN & 0x04)){

           while(!(P2IN & 0x04)) ;
           Delay_ms(10);

            mode = 2;

            tcnt = 0;

        }

        P2IFG &= ~0x04;

    }

    else if(P2IFG & 0x08){

        Delay_ms(10);

        if(!(P2IN & 0x08)){

            while(!(P2IN & 0x08)) ;
            Delay_ms(10);

            shan = 3;

            mode = 3;

            tcnt = 0;

        }

        P2IFG &= ~0x08;

    }

}



#pragma vector = TIMER0_A0_VECTOR

__interrupt void Timer0_Isr(void){



    if(mode == 0){

        P1OUT |= 0x01;      //常亮

    }

    else if(mode == 1){

        tcnt ++;

        if(tcnt >= 50) {      // 0.5s 切换亮灭

            tcnt = 0;

            P1OUT ^= 0x01;

        }

    }

    else if(mode == 2){

        tcnt ++;

        if(tcnt >= 10){         //  0.1秒切换亮灭

            tcnt = 0;

            P1OUT ^= 0x01;

        }

    }

    else if(mode == 3){

        if(shan){

            tcnt ++;

            if(tcnt < 120) P1OUT |= 0x01;  // 1.2s亮

            else if(tcnt < 200) P1OUT &= ~0x01;            // 0.8s 灭

            else if(tcnt >= 200){

                tcnt = 0;

                shan --;

                if(shan) P1OUT |= 0x01;

            }

        }

    }

}

