#include <msp430.h> 

// ����ܵĶ�ѡ�� SMG[0] - SMG[15] ��ʾ 0 - F SMG[16] �� -
// ����ǵ�������� ֻ��Ҫ P1OUT = SMG[x] ������ʾĳ����
// ����������� ���Ҫ�ù����������Ӧ��ȡ�� ~
unsigned char SMG[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,0x40};

unsigned long data;
//data ��һ��32λ�ı��� ÿ4λ���Ա�ʾһ��0-15ֱ�ӵ���������һ���������Ա�ʾ 8�� 0-15ֱ�ӵ���
// ���������������
unsigned short show[8] = {0,1,2,3,4,5,6,7};

// ��ʱ���ĳ�ʼ��
void Timer_Init(){
    TA0CCTL0 = CCIE;
    TACCR0 = 2000 - 1;  // ע�ⲻ�ܳ���65535
    TACTL = TASSEL_2 | ID_0  | MC_1 | TACLR;
    // SMCLK Ϊʱ��Դ ����Ƶ��1��Ƶ�� ���ϼ���ģʽ
    // ��ʱ��ʱ��ļ��㷽ʽ �� (TACCR0��ֵ + 1 ) / (1000000(1M) / ��Ƶ��)) ��������������� 2000 / (1000000/1) = 0.002 s
    // ID_0 - ID_3 �ķ�Ƶ���ֱ��� 1 2 4 8
    __enable_interrupt(); //�����ж�

//    // �ڶ�����ʱ��
    TA1CCTL0 = CCIE;
    TA1CCR0 = 12500 - 1;
    //(12500 / (1000000/8)) = 0.1 s
    TA1CTL = TASSEL_2 | ID_3  | MC_1 | TACLR;
    __enable_interrupt(); //�����ж�
}

// ��ʼ�����
void Init(){
    Timer_Init();

    //��� ���ｫ P1.0 - P1.7 ����ΪGPIO�������
    P1SEL = 0x00;
    P1SEL2 = 0x00; //���ó�GPIO��
    P1DIR = 0xFF; // PXDIR 1 Ϊ���ģʽ
    P1OUT = 0xFF;   // PXOUT 1 ����ߵ�ƽ������Ϊ�ƻ��������ȫ��������������ȸĳ�0x00 ȫ����Ϊ�͵�ƽ��

    // ��P3.0 - P3.7 ����Ϊ���
    P3SEL = 0x00;
    P3SEL2 = 0x00;
    P3DIR = 0xFF;
    P3OUT = 0xFF;

    //�������� �����ǽ�P2.0 - P2.3����ΪGPIO������ ��������������
    P2SEL &= ~0x0F;
    P2SEL2 &= ~0x0F; //���ó�GPIO��
    P2DIR &= ~0x0F; // PXDIR 0 Ϊ����
    P2REN |= 0x0F; // PXREN 1  ��������������
    P2OUT |= 0x0F;  // ����ģʽ�� PXOUT 1 ������������
    // �����ж�����
    P2IE |= 0x0F;   //�����ж�
    P2IES |=0x0F;   //����ΪΪ�½��ش���
    P2IFG &= ~0x0F; //����жϱ�־λ

    //����ȫ���жϣ�ʹ�õ��ն˵Ĵ��붼Ҫд���
    __enable_interrupt();
}

// Delay �ǵùر����Ż� �������2.1 ��ppt��
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

// �����жϵ�ʵ�� �����ʹ�õİ�������P2����ģ�PORT2����������
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void){
    if(P2IFG & 0x01){   //����ǲ��ǵ�һ������
        Delay_ms(10);     // ȥ��
        if(!(P2IN & 0x01)){  // �жϰ����Ƿ��Ǳ�����
            //ִ����Ĳ���
        }
        P2IFG &= ~0x01;     //����жϱ�־λ
    }
    // �ڶ���������ͬ��
    else if(P2IFG & 0x02){
        Delay_ms(10);
        if(!(P2IN & 0x02)){
            //ִ����Ĳ���
        }
        P2IFG &= ~0x02;
    }
}

int cntT0 = 0; //ÿ0.002�봥��һ���ж� ��¼�����˼���0.002����ն�
int pos = 0;
// ��ʱ���жϵ�ʵ��
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Isr(void){
    // ��ʱ���ж�ʵ��ɨ�������
    // ÿ0.002�봥��һ���жϣ�ÿ����ʾ����ܵ�һλ��0.018���ھͿ�����ʾ8λ
    P1OUT = 0x00;       // ȫ�� ��Ӱ
    P3OUT = ~(1 << pos);    //��posλΪ�ͣ���ʾ��pos��������� (����������ܣ��ĸ�λ�ǵ��ĸ���)
    P1OUT = SMG[(data >> (pos << 2)) & 0x0F];   // ��ʾ
    // P1OUT = SMG[show[pos]];   //������Ļ��������Ǿ�ĳ����������
    pos ++;
    if(pos >= 8) pos = 0;

    cntT0 ++;
    if(cntT0 >= 500){    // ����200��0.002����ж�˵������1s    >= ��Ϊ�˷�ֹ���� һ�� == �͹�
        data ++;
        cntT0 = 0;  // ��ռ�����
    }
}

// �ڶ�����ʱ���жϵ�ʵ��
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_Isr(void){
    // ִ����Ĳ���
}

/*
 while(1)�ڰ���ʶ�� ��û��Ҫ���ж�ʵ�ֵ�ʱ������ã�
    while(1){
    if((P2IN & 0x01) == 0){
            Delay_ms(10);
            if((P2IN & 0x01) == 0){
                while((P2IN & 0x01) == 0);
               //��������Ӧ��ִ�еĲ���
            }
        }
    }
 */
