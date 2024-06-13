#include <msp430.h>
#include "Typedef.h"

#define SDA_PIN BIT7 // P1.7
#define SCL_PIN BIT6 //     P1.6
#define OLED_ADDRESS 0x78

/**
 * @brief 设置SDA口高低电平s
*/
void OLED_W_SDA(int x){
    if(x) P1OUT |= SDA_PIN;
    else P1OUT &= ~SDA_PIN;
}

/**
 * @brief 设置SDA口高低电平s
*/
void OLED_W_SCL(int x){
    if(x) P1OUT |= SCL_PIN;
    else P1OUT &= ~SCL_PIN;
}

void I2C_Start(void) {
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

void I2C_Stop(void) {
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
 * @brief 发送一个字节
 * @param 发送的数据
*/
void I2C_SendByte(uint8_t byte) {
    int i;
    for (i = 0; i < 8; i++) {
        OLED_W_SCL(0);
        OLED_W_SDA(byte & (0x80 >> i));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(0);
    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

/**
 * @brief 写命令
 * @param 命令
*/
void OLED_W_CMD(uint8_t CMD) {
    I2C_Start();
    I2C_SendByte(OLED_ADDRESS);
    I2C_SendByte(0x00);
    I2C_SendByte(CMD);
    I2C_Stop();
}

/**
 * @brief 写数据
 * @param 数据
*/
void OLED_W_Data(uint8_t data){
    I2C_Start();
    I2C_SendByte(OLED_ADDRESS);
    I2C_SendByte(0x40);
    I2C_SendByte(data);
    I2C_Stop();
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
    P1SEL &= ~(SDA_PIN + SCL_PIN);
    P1SEL2 &= ~(SDA_PIN + SCL_PIN);
    P1DIR |= SDA_PIN + SCL_PIN;
    P1OUT |= SDA_PIN + SCL_PIN;

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




