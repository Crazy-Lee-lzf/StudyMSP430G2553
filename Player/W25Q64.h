#include "Typedef.h"

#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(void);
void W25Q64_R_ID(uint8_t *MID, uint16_t *DID);
void W25Q64_WriteEnable(void);
void W25Q64_WaitBusy(void);
void W25Q64_PageProgram(uint32_t Add, uint8_t *Data, uint16_t cnt);
void W25Q64_SectorErase(uint32_t Add);
void W25Q64_BlockErase32(uint32_t Add);
void W25Q64_BlockErase64(uint32_t Add);
void W25Q64_ReadData(uint32_t Add, uint8_t *Data, uint16_t cnt);

#endif
