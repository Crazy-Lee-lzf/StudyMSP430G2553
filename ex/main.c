#include <msp430.h> 

// 数码管的段选码 SMG[0] - SMG[15] 表示 0 - F SMG[16] 是 -
// 如果是单个数码管 只需要 P1OUT = SMG[x] 就能显示某个数
// 共阴极数码管 如果要用共阳极数码管应该取反 ~
unsigned char SMG[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,0x40};

unsigned long data;
//data 是一个32位的变量 每4位可以表示一个0-15直接的数，所以一个变量可以表示 8个 0-15直接的数
// 看不懂就用数组吧
unsigned short show[8] = {0,1,2,3,4,5,6,7};

// 定时器的初始化
void Timer_Init(){
    TA0CCTL0 = CCIE;
    TACCR0 = 2000 - 1;  // 注意不能超过65535
    TACTL = TASSEL_2 | ID_0  | MC_1 | TACLR;
    // SMCLK 为时钟源 不分频（1分频） 向上计数模式
    // 定时器时间的计算方式 ： (TACCR0的值 + 1 ) / (1000000(1M) / 分频数)) 代入上面这个例子 2000 / (1000000/1) = 0.002 s
    // ID_0 - ID_3 的分频数分别是 1 2 4 8
    __enable_interrupt(); //启用中断

//    // 第二个定时器
    TA1CCTL0 = CCIE;
    TA1CCR0 = 12500 - 1;
    //(12500 / (1000000/8)) = 0.1 s
    TA1CTL = TASSEL_2 | ID_3  | MC_1 | TACLR;
    __enable_interrupt(); //启用中断
}

// 初始化相关
void Init(){
    Timer_Init();

    //输出 这里将 P1.0 - P1.7 设置为GPIO的输出口
    P1SEL = 0x00;
    P1SEL2 = 0x00; //设置成GPIO口
    P1DIR = 0xFF; // PXDIR 1 为输出模式
    P1OUT = 0xFF;   // PXOUT 1 输出高电平（表现为灯或者数码管全亮，有需求可以先改成0x00 全设置为低电平）

    // 将P3.0 - P3.7 设置为输出
    P3SEL = 0x00;
    P3SEL2 = 0x00;
    P3DIR = 0xFF;
    P3OUT = 0xFF;

    //按键设置 这里是将P2.0 - P2.3配置为GPIO的输入 并启用上拉电阻
    P2SEL &= ~0x0F;
    P2SEL2 &= ~0x0F; //配置成GPIO口
    P2DIR &= ~0x0F; // PXDIR 0 为输入
    P2REN |= 0x0F; // PXREN 1  启用上下拉电阻
    P2OUT |= 0x0F;  // 输入模式下 PXOUT 1 启用上拉电阻
    // 按键中断设置
    P2IE |= 0x0F;   //启用中断
    P2IES |=0x0F;   //设置为为下降沿触发
    P2IFG &= ~0x0F; //清除中断标志位

    //开启全局中断，使用的终端的代码都要写这个
    __enable_interrupt();
}

// Delay 记得关编译优化 （具体见2.1 的ppt）
void Delay_ms(int t){
    int n;
    while(t--){
        n = 100;
        while(n--) ;
    }
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Init();
	while(1){
	    ;
	}
}

// 按键中断的实现 如果你使用的按键不是P2里面的，PORT2换成其他的
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x01){   //检测是不是第一个按键
        Delay_ms(10);     // 去抖
        if(!(P2IN & 0x01)){  // 判断按键是否还是被按下
            //执行你的操作
        }
        P2IFG &= ~0x01;     //清除中断标志位
    }
    // 第二个按键，同理
    else if(P2IFG & 0x02){
        Delay_ms(10);
        if(!(P2IN & 0x02)){
            //执行你的操作
        }
        P2IFG &= ~0x02;
    }
}

int cntT0 = 0; //每0.002秒触发一次中断 记录发送了几个0.002秒的终端
int pos = 0;
// 定时器中断的实现
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
    // 定时器中断实现扫描数码管
    // 每0.002秒触发一次中断，每次显示数码管的一位，0.018秒内就可以显示8位
    P1OUT = 0x00;       // 全亮 消影
    P3OUT = ~(1 << pos);    //第pos位为低，表示第pos个数码管亮 (共阴极数码管，哪个位是低哪个亮)
    P1OUT = SMG[(data >> (pos << 2)) & 0x0F];   // 显示
    // P1OUT = SMG[show[pos]];   //用数组的话吧上面那句改成这个就行了
    pos ++;
    if(pos >= 8) pos = 0;

    cntT0 ++;
    if(cntT0 >= 500){    // 发生200次0.002秒的中断说明过了1s    >= 是为了防止意外 一般 == 就够
        data ++;
        cntT0 = 0;  // 清空计数器
    }
}

// 第二个定时器中断的实现
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_Isr(void){
    // 执行你的操作
}

/*
 while(1)内按键识别 （没有要求中断实现的时候可以用）
    while(1){
    if((P2IN & 0x01) == 0){
            Delay_ms(10);
            if((P2IN & 0x01) == 0){
                while((P2IN & 0x01) == 0);
               //按键按下应该执行的操作
            }
        }
    }
 */
