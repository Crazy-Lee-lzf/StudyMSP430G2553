#include <msp430.h>

#define OLED_ADDRESS 0x3C

void i2c_init() {
    // ��ʼ��I2CӲ��ģ��
    // ���ö˿ں���������I2Cͨ��
    // ����ʱ�Ӻ����ʿ���
    P1SEL |= BIT6 + BIT7; // P1.6ΪSCL��P1.7ΪSDA
    P1SEL2 |= BIT6 + BIT7;
    UCB0CTL1 |= UCSWRST; // �����λUCB0������
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // ��ģʽ��I2Cģʽ��ͬ��ͨ��
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // ѡ��SMCLK�����������λ
    UCB0BR0 = 10; // ����ʱ��Ƶ��ΪSMCLK/10����100kHz
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST; // ���UCB0�����λ
}

void i2c_start() {
    // ������ʼ�ź�
    UCB0CTL1 |= UCTR + UCTXSTT; // ������ģʽ��������ʼ�ź�
    while (UCB0CTL1 & UCTXSTT); // �ȴ���ʼ�źŷ������
}

void i2c_stop() {
    // ����ֹͣ�ź�
    UCB0CTL1 |= UCTXSTP; // ����ֹͣ�ź�
    while (UCB0CTL1 & UCTXSTP); // �ȴ�ֹͣ�źŷ������
}

void i2c_write_byte(unsigned char data) {
    // ����һ���ֽڵ�����
    UCB0TXBUF = data; // ������д��TX������
    while (!(UCB0IFG & UCTXIFG)); // �ȴ�TX������Ϊ��
}

void oled_command(unsigned char command) {
    // ����OLED������
    i2c_start();
    i2c_write_byte(OLED_ADDRESS);
    i2c_write_byte(0x00); // Command mode
    i2c_write_byte(command);
    i2c_stop();
}

void oled_init() {
    // OLED��ʼ������
    oled_command(0xAE); // Display off
    oled_command(0xA4); // Entire display ON
    oled_command(0xAF); // Display on
}

void main(void) {
    // ͣ�ÿ��Ź�
    WDTCTL = WDTPW + WDTHOLD;

    // ����ʱ��
    BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;

    // ��ʼ��I2C
    i2c_init();

    // ��ʼ��OLED
    oled_init();

    while(1) {
        // ����ѭ��
    }
}
