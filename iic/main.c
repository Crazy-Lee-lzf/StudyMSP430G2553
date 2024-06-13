#include <msp430.h>

#define OLED_ADDRESS 0x3C

void i2c_init() {
    // 初始化I2C硬件模块
    // 设置端口和引脚用于I2C通信
    // 设置时钟和速率控制
    P1SEL |= BIT6 + BIT7; // P1.6为SCL，P1.7为SDA
    P1SEL2 |= BIT6 + BIT7;
    UCB0CTL1 |= UCSWRST; // 软件复位UCB0控制器
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // 主模式，I2C模式，同步通信
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // 选择SMCLK，保持软件复位
    UCB0BR0 = 10; // 设置时钟频率为SMCLK/10，即100kHz
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST; // 清除UCB0软件复位
}

void i2c_start() {
    // 发送起始信号
    UCB0CTL1 |= UCTR + UCTXSTT; // 发送器模式，发送起始信号
    while (UCB0CTL1 & UCTXSTT); // 等待起始信号发送完成
}

void i2c_stop() {
    // 发送停止信号
    UCB0CTL1 |= UCTXSTP; // 发送停止信号
    while (UCB0CTL1 & UCTXSTP); // 等待停止信号发送完成
}

void i2c_write_byte(unsigned char data) {
    // 发送一个字节的数据
    UCB0TXBUF = data; // 将数据写入TX缓冲区
    while (!(UCB0IFG & UCTXIFG)); // 等待TX缓冲区为空
}

void oled_command(unsigned char command) {
    // 发送OLED的命令
    i2c_start();
    i2c_write_byte(OLED_ADDRESS);
    i2c_write_byte(0x00); // Command mode
    i2c_write_byte(command);
    i2c_stop();
}

void oled_init() {
    // OLED初始化序列
    oled_command(0xAE); // Display off
    oled_command(0xA4); // Entire display ON
    oled_command(0xAF); // Display on
}

void main(void) {
    // 停用看门狗
    WDTCTL = WDTPW + WDTHOLD;

    // 设置时钟
    BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;

    // 初始化I2C
    i2c_init();

    // 初始化OLED
    oled_init();

    while(1) {
        // 无限循环
    }
}
