#include <msp430.h> 
#include "OLED.h"

void Init(void){
    OLED_Init();
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Init();
	while(1){
	    OLED_Clear();
	}
}
