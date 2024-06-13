#include <msp430.h>
#include "W25Q64_Ins.h"
#include "MySPI.h"
#include "Typedef.h"

void W25Q64_Init(void){
    MySPI_Init();
}

/**
 * @brief 读取设备ID和厂商ID
 * @param 厂商ID地址
 * @param 设备ID地址
*/
void W25Q64_R_ID(uint8_t *MID, uint16_t *DID){
    MySPI_Start();
    MySPI_SwapByte(W25Q64_JEDEC_ID);    //获取ID
    *MID = MySPI_SwapByte(0xFF);
    *DID = MySPI_SwapByte(0xFF);
    *DID <<= 8;
    *DID |= MySPI_SwapByte(0xFF);
}

/**
 * @brief 写使能
*/
void W25Q64_WriteEnable(void){
    MySPI_Start();
    MySPI_SwapByte(W25Q64_WRITE_ENABLE);
    MySPI_Stop();
}

/**
 * @brief 等待写入
*/
void W25Q64_WaitBusy(void){
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
    while( (MySPI_SwapByte(0xFF) & 0x01) == 0x01);
    MySPI_Stop();
}

/**
 * @brief 存数据
 * @param 要存的数据数组
 * @param 数组的长度
*/
void W25Q64_PageProgram(uint32_t Add, uint8_t *Data, uint16_t cnt){
    W25Q64_WriteEnable();

    MySPI_Start();
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
    MySPI_SwapByte(Add >> 16);
    MySPI_SwapByte(Add >> 8);
    MySPI_SwapByte(Add);
    
    uint16_t i;
    for(i = 0;i < cnt;i ++){
        MySPI_SwapByte(Data[i]);
    }
    MySPI_Stop();

    W25Q64_WaitBusy();
}

/**
 * @brief 清除一个扇区(4K)
 * @param 地址
*/
void W25Q64_SectorErase(uint32_t Add){
    W25Q64_WriteEnable();

    MySPI_Start();
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
    MySPI_SwapByte(Add >> 16);
    MySPI_SwapByte(Add >> 8);
    MySPI_SwapByte(Add);
    MySPI_Stop();

    W25Q64_WaitBusy();
}

/**
 * @brief 清除一个Block(32k)
 * @param 地址
*/
void W25Q64_BlockErase32(uint32_t Add){
    W25Q64_WriteEnable();

    MySPI_Start();
    MySPI_SwapByte(W25Q64_BLOCK_ERASE_32KB);
    MySPI_SwapByte(Add >> 16);
    MySPI_SwapByte(Add >> 8);
    MySPI_SwapByte(Add);
    MySPI_Stop();

    W25Q64_WaitBusy();
}

/**
 * @brief 清除一个Block(32k)
 * @param 地址
*/
void W25Q64_BlockErase64(uint32_t Add){
    W25Q64_WriteEnable();
    MySPI_Start();
    MySPI_SwapByte(W25Q64_BLOCK_ERASE_64KB);
    MySPI_SwapByte(Add >> 16);
    MySPI_SwapByte(Add >> 8);
    MySPI_SwapByte(Add);
    MySPI_Stop();

    W25Q64_WaitBusy();
}

/**
 * @brief 读取数据
 * @param 存数据的数组
 * @param 数据大小
*/
void W25Q64_ReadData(uint32_t Add, uint8_t *Data, uint16_t cnt){
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_DATA);
    MySPI_SwapByte(Add >> 16);
    MySPI_SwapByte(Add >> 8);
    MySPI_SwapByte(Add);
    
    uint16_t i;
    for(i = 0;i < cnt;i ++){
        Data[i] = MySPI_SwapByte(0xFF);
    }

    MySPI_Stop();
}

