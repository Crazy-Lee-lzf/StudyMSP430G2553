#include <msp430.h> 
#include "KeyBorad.h"

/**
 * main.c
 */
int resbit(int x){
    int ans=0, i = 0;
    for(i = 0;i < 7;i++){
        ans |= (x & 0x01);
        ans = ans << 1;
        x = x >> 1;
    }
    return ans;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	KeyBoradInit();

	P1DIR |= 0xFF;
	int key = 0;
	while(1){
	    key = GetKeyBoradValue();
	    if(key != 0){
	        P1OUT = resbit(key);
	    }
	}

}
