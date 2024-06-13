#include "Typedef.h"

#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowFull(char *s);
void OLED_W_CMD(uint8_t CMD);
void OLED_W_Data(uint8_t data);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, char Char);
void OLED_ShowString(uint8_t x, uint8_t y, char *String);
void OLED_Show_Ch(uint8_t x, uint8_t y, uint8_t *s, uint8_t fan);
void OLED_Show_ChLine(uint8_t Line, char *s, uint8_t fan, int *gun);

#endif
