/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>
#define Delay_us(x) __delay_cycles((long)x);
#define Delay_ms(x) __delay_cycles((long)x * 1000);

/*
 * ======== Grace related declaration ========
 */
extern void Grace_init(void);

/*
 *  ======== main ========
 */


void Send(char *p){
    while(*p != '\0'){
        while(!(UCA0TXIFG));
        UCA0TXBUF = *p;
        p++;
        Delay_ms(10);
    }
}

char s[20];

int main( void )
{
    Grace_init();                     // Activate Grace-generated configuration
    P2SEL &= ~0x80;
    P2SEL2 &= ~0x80; //配置成GPIO口
    P2DIR &= ~0x80; // PXDIR 0 为输入
    P2REN |= 0x80; // PXREN 1  启用上下拉电阻
    P2OUT |= 0x80;  // 输入模式下 PXOUT 1 启用上拉电阻

    P2IE |= 0x80;   //启用中断
    P2IES |=0x80;   //设置为为下降沿触发
    P2IFG &= ~0x80; //清除中断标志

    while(1){
        Send("Hello World");
        Delay_ms(2000);
    };
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x80){   //检测是不是第一个按键
        Delay_ms(10);     // 去抖
        if(!(P2IN & 0x80)){  // 判断按键是否还是被按下
            Send("6666");
        }
        P2IFG &= ~0x80;     //清除中断标志位
    }
}
