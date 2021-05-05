/*
* twi.h
*
* Created: 05/05/2021 16:29:28
*  Author: Moe
*/


#ifndef TWI_H_
	#define TWI_H_
#include <stdio.h>
#include <stdint.h>

void TWIInit(void);
void TWIStart(void);
void TWIStop(void);
void TWIWrite(uint8_t u8data);
uint8_t TWIReadACK(void);
uint8_t TWIReadNACK(void);
uint8_t TWIGetStatus(void);

#endif /* TWI_H_ */