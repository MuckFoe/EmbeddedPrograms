/*
* eeProm.c
*
* Created: 05/05/2021 17:52:06
*  Author: Moe
*/
#include <stdio.h>
#include <stdint.h>
#include "twi.h"

#define SLA_W 0b10100000
#define SLA_R 0b10100001

uint8_t EEWriteByte(uint8_t u8addr, uint8_t u8data)
{
	TWIStart();
	if (TWIGetStatus() != 0x08){
		printf("error \r\n");
	}
	//select devise and send A2 A1 A0 address bits
	TWIWrite(SLA_W);
	if (TWIGetStatus() != 0x18){
		printf("error \r\n");
	}
	//send the rest of address
	TWIWrite((uint8_t)(u8addr));
	if (TWIGetStatus() != 0x28){
		printf("error \r\n");
	}
	//write byte to eeprom
	TWIWrite(u8data);
	if (TWIGetStatus() != 0x28){
		printf("error \r\n");
	}
	TWIStop();
	printf("success \r\n");
	return 0;
}


uint8_t EEReadByte(uint8_t u8addr, uint8_t *u8data)
{
	//uint8_t databyte;
	TWIStart();
	if (TWIGetStatus() != 0x08){
		printf("error \r\n");
	}
	//select devise and send A2 A1 A0 address bits
	TWIWrite(SLA_W);
	if (TWIGetStatus() != 0x18){
		printf("error \r\n");
	}
	//send the rest of address
	TWIWrite((uint8_t)(u8addr));
	if (TWIGetStatus() != 0x28){
		printf("error \r\n");
	}
	//send start
	TWIStart();
	if (TWIGetStatus() != 0x10){
		printf("error \r\n");
	}
	//select devise and send read bit
	TWIWrite(SLA_R);
	if (TWIGetStatus() != 0x40){
		printf("error \r\n");
	}
	*u8data = TWIReadNACK();
	if (TWIGetStatus() != 0x58){
		printf("error \r\n");
	}
	TWIStop();
	printf("success \r\n");
	return 0;
}

uint8_t EEWritePage(uint8_t page, uint8_t *u8data)
{
	//calculate page address
	uint8_t u8paddr = 0;
	uint8_t i;
	u8paddr = page<<4;
	TWIStart();
	if (TWIGetStatus() != 0x08){
		printf("error \r\n");
		return TWIGetStatus();
	}
	//select page start address and send A2 A1 A0 bits send write command
	TWIWrite(SLA_W);
	if (TWIGetStatus() != 0x18){
		printf("error \r\n");
		return TWIGetStatus();
	}
	//send the rest of address
	TWIWrite((u8paddr<<4));
	if (TWIGetStatus() != 0x28){
		printf("error \r\n");
		return TWIGetStatus();
	}
	//write page to eeprom
	for (i=0; i<16; i++)
	{
		TWIWrite(*u8data++);
		if (TWIGetStatus() != 0x28){
			printf("error \r\n");
			return TWIGetStatus();
		}
	}
	TWIStop();
	printf("success \r\n");
	return 0;
}

uint8_t EEReadPage(uint8_t page, uint8_t *u8data)
{
	//calculate page address
	uint8_t u8paddr = 0;
	uint8_t i;
	u8paddr = page<<4;
	TWIStart();
	if (TWIGetStatus() != 0x08){
		printf("start error \r\n");
		return TWIGetStatus();
	}
	//select page start address and send A2 A1 A0 bits send write command
	TWIWrite(SLA_W);
	if (TWIGetStatus() != 0x18){
		printf("error  writing init\r\n");
		return TWIGetStatus();
	}
	//send the rest of address
	TWIWrite((u8paddr<<4));
	if (TWIGetStatus() != 0x28){
		printf("error writing address \r\n");
		return TWIGetStatus();
	}
	//send start
	TWIStart();
	if (TWIGetStatus() != 0x10){
		printf("error starting transmission before read \r\n");
		return TWIGetStatus();
	}
	//select device and send read bit
	TWIWrite(SLA_R);
	if (TWIGetStatus() != 0x40){
		printf("error \r\n");
		return TWIGetStatus();
		
	}
	for (i=0; i<15; i++)
	{
		*u8data++ = TWIReadACK();
		if (TWIGetStatus() != 0x50)
		printf("error reading data \r\n");
		return TWIGetStatus();
	}
	*u8data = TWIReadNACK();
	if (TWIGetStatus() != 0x58){
		printf("error end of data \r\n");
		return TWIGetStatus();
	}
	TWIStop();
	printf("success \r\n");
	return 0;
}
