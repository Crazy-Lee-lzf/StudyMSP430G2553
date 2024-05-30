/*
 * Delay.h
 *
 *  Created on: 2024Äê3ÔÂ23ÈÕ
 *      Author: Administrator
 */

#ifndef DELAY_H_
#define DELAY_H_

#define Delay_us(x) __delay_cycles((long)x);
#define Delay_ms(x) __delay_cycles((long)x * 1000)

#endif /* DELAY_H_ */
