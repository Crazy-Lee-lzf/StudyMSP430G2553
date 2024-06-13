#include <msp430.h>
#include "Typedef.h"

#define SDA_PIN 0x80 // P1.7
#define SCL_PIN 0x40 // P1.6

#define SLAVE_ADDR 0x60

void MCP4075_WriteData(uint16_t Data) {
    while (UCB0CTL1 & UCTXSTP); // 等待停止条件传输完成
        UCB0CTL1 |= UCTR + UCTXSTT; // 发送起始条件和传输模式
    while (!(IFG2 & UCB0TXIFG)); // 等待发送缓冲区空闲
        UCB0TXBUF = Data >> 8; // 将数据写入发送缓冲区
    while (!(IFG2 & UCB0TXIFG)); // 等待发送缓冲区空闲
        UCB0TXBUF = Data & 0xFF; // 将数据写入发送缓冲区
    while (!(IFG2 & UCB0TXIFG)); // 等待发送完成
        UCB0CTL1 |= UCTXSTP; // 发送停止条件

    while (UCB0CTL1 & UCTXSTP); // 等待停止条件传输完成
}

void MCP4075_Init(void) {
    P1SEL |= SDA_PIN + SCL_PIN; // 将P1.6和P1.7设置为I2C功能
    P1SEL2|= SDA_PIN + SCL_PIN;

    UCB0CTL1 |= UCSWRST; // 进入软件复位状态
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // 主模式，I2C模式，同步模式
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // 使用SMCLK
    UCB0BR0 = 2; // 设置波特率（根据实际情况调整）
    UCB0BR1 = 0;
    UCB0I2CSA = SLAVE_ADDR; // 设置从设备地址
    UCB0CTL1 &= ~UCSWRST; // 退出软件复位状态
}
