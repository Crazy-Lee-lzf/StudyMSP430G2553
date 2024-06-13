#include <msp430.h>
#include "Typedef.h"

#define SDA_PIN BIT7 // P1.7
#define SCL_PIN BIT6 //     P1.6
#define OLED_ADDRESS 0x3C


/**
 * @brief 写命令
 * @param 命令
*/
void OLED_W_CMD(uint8_t CMD) {
    while (UCB0CTL1 & UCTXSTP); // 等待停止条件传输完成
        UCB0CTL1 |= UCTR + UCTXSTT; // 发送起始条件和传输模式
    while (!(IFG2 & UCB0TXIFG)); // 等待发送缓冲区空闲
        UCB0TXBUF = 0x00; // 将数据写入发送缓冲区
    while (!(IFG2 & UCB0TXIFG)); // 等待发送缓冲区空闲
        UCB0TXBUF = CMD; // 将数据写入发送缓冲区
    while (!(IFG2 & UCB0TXIFG)); // 等待发送完成
        UCB0CTL1 |= UCTXSTP; // 发送停止条件

    while (UCB0CTL1 & UCTXSTP); // 等待停止条件传输完成
}

/**
 * @brief 写数据
 * @param 数据
*/
void OLED_W_Data(uint8_t data){
    while (UCB0CTL1 & UCTXSTP); // 等待停止条件传输完成
        UCB0CTL1 |= UCTR + UCTXSTT; // 发送起始条件和传输模式
    while (!(IFG2 & UCB0TXIFG)); // 等待发送缓冲区空闲
        UCB0TXBUF = 0x40; // 将数据写入发送缓冲区
    while (!(IFG2 & UCB0TXIFG)); // 等待发送缓冲区空闲
        UCB0TXBUF = data; // 将数据写入发送缓冲区
    while (!(IFG2 & UCB0TXIFG)); // 等待发送完成
        UCB0CTL1 |= UCTXSTP; // 发送停止条件

    while (UCB0CTL1 & UCTXSTP); // 等待停止条件传输完成
}

/**
 * @brief 开启显示
*/
void OLED_DisplayOn(void){
    OLED_W_CMD(0x8D);
    OLED_W_CMD(0x14);
    OLED_W_CMD(0xAF);
}

/**
 * @brief 关闭显示
*/
void OLED_DisplayOff(void){
    OLED_W_CMD(0x8D);
    OLED_W_CMD(0x10);
    OLED_W_CMD(0xAE);
}
/**
 * @brief 设置位置
 * @param 第几行 0 - 7
 * @param 第几列 0 - 127
*/
void OLED_SetPos(uint8_t y, uint8_t x){
    OLED_W_CMD(0xB0 | y);
    OLED_W_CMD(((x & 0xF0) >> 4) | 0x10);
    OLED_W_CMD((x & 0x0F) | 0x01);
}
/**
  * @brief  清屏
  */
void OLED_Clear(void){
    int i, j;
    for(i = 0;i < 8;i++){
        OLED_SetPos(i, 0);
        for(j = 0;j < 128;j++){
            OLED_W_Data(0xF0);
        }
    }
}

/**
  * @brief  显示单个字符
  * @param  行
  * @param  列
  * @param  字符数组
  * @param  是否反显
  */
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t *s, uint8_t fan){
    uint8_t i;
    OLED_SetPos((x - 1) * 2, (y - 1) * 16);
    for(i = 0;i < 16;i++){
        if(fan) OLED_W_Data(~(*s));
        else OLED_W_Data(*s);
        s++;
    }
    OLED_SetPos((x - 1) * 2 + 1, (y - 1) * 16);
    for(i = 16; i < 32;i++){
        if(fan) OLED_W_Data(~(*s));
        else OLED_W_Data(*s);
        s++;
    }
}

/*
 * @brief 初始化
*/
void OLED_Init(void) {
    P1SEL |= SDA_PIN + SCL_PIN; // 将P1.6和P1.7设置为I2C功能
    P1SEL2|= SDA_PIN + SCL_PIN;

    UCB0CTL1 |= UCSWRST; // 进入软件复位状态
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // 主模式，I2C模式，同步模式
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // 使用SMCLK
    UCB0BR0 = 12; // 设置波特率（根据实际情况调整）
    UCB0BR1 = 0;
    UCB0I2CSA = OLED_ADDRESS; // 设置从设备地址
    UCB0CTL1 &= ~UCSWRST; // 退出软件复位状态

    OLED_W_CMD(0xAE);    //关闭显示

    OLED_W_CMD(0xD5);    //设置显示时钟分频比/振荡器频率
    OLED_W_CMD(0x80);

    OLED_W_CMD(0xA8);    //设置多路复用率
    OLED_W_CMD(0x3F);

    OLED_W_CMD(0xD3);    //设置显示偏移
    OLED_W_CMD(0x00);

    OLED_W_CMD(0x40);    //设置显示开始行

    OLED_W_CMD(0xDA);    //设置COM引脚硬件配置
    OLED_W_CMD(0x12);

    OLED_W_CMD(0x81);    //设置对比度控制
    OLED_W_CMD(0xCF);

    OLED_W_CMD(0xD9);    //设置预充电周期
    OLED_W_CMD(0xF1);

    OLED_W_CMD(0xDB);    //设置VCOMH取消选择级别
    OLED_W_CMD(0x30);

    OLED_W_CMD(0xA4);    //设置整个显示打开/关闭

    OLED_W_CMD(0xA6);    //设置正常/倒转显示

    OLED_W_CMD(0x8D);    //设置充电泵
    OLED_W_CMD(0x14);

    OLED_W_CMD(0xAF);    //开启显示

    OLED_W_CMD(0xA1);    //设置左右方向，0xA1正常 0xA0左右反置
    OLED_W_CMD(0xC8);    //设置上下方向，0xC8正常 0xC0上下反置

    OLED_Clear();
}




