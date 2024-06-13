#include <msp430.h>
#include "Typedef.h"

#define MOSI_PIN_A0 BIT2 // P1.2
#define MISO_PIN_A0 BIT1 // P1.1
#define SCLK_PIN_A0 BIT4 // P1.4
#define CS_PIN      BIT0 // P1.0

void MySPI_Start(void){
    P1OUT &= ~CS_PIN;
}

void MySPI_Stop(void){
    P1OUT |= CS_PIN;
}

void MySPI_Init(void){
    P1SEL |= MOSI_PIN_A0 + MISO_PIN_A0 + SCLK_PIN_A0; // 将P1.1, P1.2和P1.4设置为SPI功能
    P1SEL2 |= MOSI_PIN_A0 + MISO_PIN_A0 + SCLK_PIN_A0;

    P1SEL &= ~CS_PIN;
    P1DIR |= CS_PIN; // 将P1.0设置为输出方向
    P1OUT |= CS_PIN; // 将CS拉高

    UCA0CTL1 |= UCSWRST; // 进入软件复位状态
    UCA0CTL0 = UCMST + UCSYNC + UCCKPH + UCMSB; // 主模式，同步模式，相位，MSB优先
    UCA0CTL1 = UCSSEL_2 + UCSWRST; // 使用SMCLK

    UCA0BR0 = 0x01; // 设置波特率分频器
    UCA0BR1 = 0x00; // 设置波特率分频器

    UCA0CTL1 &= ~UCSWRST; // 退出软件复位状态
}

/**
 * @brief 交换一个字节
 * @param 发送的字节
 * @retval 接受的字节
*/
uint8_t MySPI_SwapByte(uint8_t Byte){
    while (!(IFG2 & UCA0TXIFG)); // 等待发送缓冲区空闲
    UCA0TXBUF = Byte; // 将数据写入发送缓冲区
    while (UCA0STAT & UCBUSY); // 等待传输完成
    return UCA0RXBUF; // 返回接收到的数据
}

