/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* Bare minimum empty user application template
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# "Insert system clock initialization code here" comment
* -# Minimal main function that starts with a call to board_init()
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "usart.h"
#include "eeProm.h"



#define F_CPU 8000000UL



int main (void)
{
	USART_Init();
	usart_setup_stdio_stream();
	
	uint8_t u8ebyte;
	uint8_t u8eaddress = 0x5E;
	uint8_t page = 5;
	uint8_t i;
	uint8_t eereadpage[16];
	uint8_t eewritepage[16] = { 10, 44, 255, 46, 80, 87, 43, 130,
	210, 23, 1, 58, 46, 150, 12, 46 };
	
	DDRB = 0xFF;
	PORTB = 0x00;
	printf("Starting \r\n");
	TWIInit();
	
	printf("Write byte From eeprom \r\n");
	printf("Write byte %#04x to eeprom address %#04x \r\n", 0x58, u8eaddress);
	printf("Result of WriteByte %#04x \r\n",EEWriteByte(u8eaddress, 0x58));
	
	printf("Read byte From eeprom \r\n");
	printf("Result of ReadByte %#04x \r\n",EEReadByte(u8eaddress, &u8ebyte));
	printf("ByteAddress: %#04x is value: %#04x \r\n", u8eaddress, u8ebyte);
	
	printf("Write 16 byte to eeprom page %i \r\n", page);
	printf("Result of WritePage %#04x \r\n",EEWritePage(page, eewritepage));
	
	
	printf("Read 16 byte to eeprom page %i \r\n", page);
	printf("Result of WritePage %#04x \r\n",EEReadPage(page, eereadpage));
	for (i=0; i<16; i++)
	{
		if (eereadpage[i] != eewritepage[i])
		{
			printf("%i \r\n", eereadpage[i]);
			break;
		}
		else continue;
	}
	if (i==16){
		printf_P(PSTR("\nPage write and read success!"));
	}
	else{
		printf_P(PSTR("\nPage write and read fail!"));
		
	}
}