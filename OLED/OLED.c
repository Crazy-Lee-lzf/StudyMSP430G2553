#include <msp430.h>

#define Slave 0x3C

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;


void OLED_W_SCL(int x){
    if(x == 1) P1OUT |= 0x01;
    else P1OUT &= ~0x01;
}

void OLED_W_SDA(int x){
    if(x == 1) P1OUT |= 0x02;
    else P1OUT &= ~0x02;
}
// 引脚配置初始化
void OLED_I2C_Init(void){
    P1SEL &= ~0x03;
    P1SEL2 &= ~0x03;
    P1DIR |= 0x03;
    
    OLED_W_SCL(1);
	OLED_W_SDA(1);
}

void OLED_I2C_Start(void){
    OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

void OLED_I2C_Stop(void){
    OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

//发送单个字节
void OLED_I2C_SendByte(uint8_t Byte){
    uint8_t i;
    for(i = 0;i < 8;i++){
        OLED_W_SDA(Byte & (0x80 >> i));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

//写命令
void OLED_W_CMD(uint8_t CMD){
    OLED_I2C_Start();
    OLED_I2C_SendByte(Slave);    //从机地址
    OLED_I2C_SendByte(0x00);    //写命令
    OLED_I2C_SendByte(CMD);
    OLED_I2C_Stop();
}

//写数据
void OLED_W_Data(uint8_t Data){
    OLED_I2C_Start();
    OLED_I2C_SendByte(Slave);
    OLED_I2C_SendByte(0x40);    //写数据
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
}

//设置写入的位置    Y表示行 X表示列
void OLED_SetPos(uint8_t Y, uint8_t X){
    OLED_W_CMD(0xB0 | Y);
    OLED_W_CMD(0x10 | ((X & 0xF0) >> 4));   //高四位
    OLED_W_CMD(0x00 | (X & 0x0F));          //低四位
}

void OLED_Clear(void){
    uint8_t i, j;
    for(i = 0;i < 8;i++){
        OLED_SetPos(i, 0);
        for(j = 0;j < 128;j++){
            OLED_W_Data(0x0F);
        }
    }
}

//OLED 初始化

void OLED_Init(void)
{
	uint16_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	OLED_I2C_Init();

	OLED_W_CMD(0xAE);//--turn off oled panel
    OLED_W_CMD(0x00);//---set low column address
    OLED_W_CMD(0x10);//---set high column address
    OLED_W_CMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_W_CMD(0x81);//--set contrast control register
    OLED_W_CMD(0xCF);// Set SEG Output Current Brightness
    OLED_W_CMD(0xA1);////--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_W_CMD(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_W_CMD(0xA6);//--set normal display
    OLED_W_CMD(0xA8);//--set multiplex ratio(1 to 64)
    OLED_W_CMD(0x3f);//--1/64 duty
    OLED_W_CMD(0xD3);//-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_W_CMD(0x00);//-not offset
    OLED_W_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_W_CMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_W_CMD(0xD9);//--set pre-charge period
    OLED_W_CMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_W_CMD(0xDA);//--set com pins hardware configuration
    OLED_W_CMD(0x12);
    OLED_W_CMD(0xDB);//--set vcomh
    OLED_W_CMD(0x30);//Set VCOM Deselect Level
    OLED_W_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_W_CMD(0x02);//
    OLED_W_CMD(0x8D);//--set Charge Pump enable/disable
    OLED_W_CMD(0x14);//--set(0x10) disable
    OLED_Clear();
    OLED_W_CMD(0xAF);
}

