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
    P2SEL2 &= ~0x80; //���ó�GPIO��
    P2DIR &= ~0x80; // PXDIR 0 Ϊ����
    P2REN |= 0x80; // PXREN 1  ��������������
    P2OUT |= 0x80;  // ����ģʽ�� PXOUT 1 ������������

    P2IE |= 0x80;   //�����ж�
    P2IES |=0x80;   //����ΪΪ�½��ش���
    P2IFG &= ~0x80; //����жϱ�־

    while(1){
        Send("Hello World");
        Delay_ms(2000);
    };
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x80){   //����ǲ��ǵ�һ������
        Delay_ms(10);     // ȥ��
        if(!(P2IN & 0x80)){  // �жϰ����Ƿ��Ǳ�����
            Send("6666");
        }
        P2IFG &= ~0x80;     //����жϱ�־λ
    }
}
