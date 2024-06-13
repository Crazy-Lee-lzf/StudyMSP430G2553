#include "Typedef.h"

#ifndef __MySPI_H
#define __MySPI_H

void MySPI_Init(void);
void MySPI_W_CS(uint8_t x);
void MySPI_W_SCL(uint8_t x);
void MySPI_W_MOSI(uint8_t x);
uint8_t MySPI_R_MISO(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t Byte);

#endif
