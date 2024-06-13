#include <msp430.h>
#include "MCP4075.h"
#include "Typedef.h"
//#include "OLED_Font.h"

void Init(){
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    MCP4075_Init();
}
int main(void) {
    Init();
    MCP4075_WriteData(4095);
    while(1);
}
