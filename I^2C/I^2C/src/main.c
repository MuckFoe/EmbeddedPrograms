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
#include <asf.h>
#include <avr/io.h>
#include "Usart.h"
#include <avr/pgmspace.h>
#define EEDEVADR 0b10100000

#define I2C_SCK 100
#define F_CPU 8000000UL

void TWIInit(void)
{
	//set SCL to 400kHz
	TWSR = 0x00;
	TWBR = 0x0C;
	TWBR = F_CPU / I2C_SCK - 16;
	//enable TWI
	TWCR = (1<<TWEN);
}

void TWIStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}
//send stop signal
void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

/*
char i2c_slaw_tx(void) {
TWDR = SLA_W; //CHIP_ADDR //Load SLA_W into TWDR Register.
TWCR = (1<<TWINT) | (1<<TWEN); //Clear TWINT bit in TWCR to start transmission
while (!(TWCR & (1<<TWINT))) //of address
;
return ((TWSR & 0xF8) == 0x18); // == MT_SLA_ACK, SLA+W has been transmitted; ACK received
}
*/
void TWIWrite(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);

}

uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}
//read byte with NACK
uint8_t TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t TWIGetStatus(void)
{
	uint8_t status;
	//mask status
	status = TWSR & 0xF8;
	return status;
}

uint8_t EEWriteByte(uint16_t u16addr, uint8_t u8data)
{
	TWIStart();
	if (TWIGetStatus() != 0x08)
	printf("error \r\n");
	//select devise and send A2 A1 A0 address bits
	TWIWrite((EEDEVADR)|(uint8_t)((u16addr & 0x0700)>>7));
	if (TWIGetStatus() != 0x18)
	printf("error \r\n");;
	//send the rest of address
	TWIWrite((uint8_t)(u16addr));
	if (TWIGetStatus() != 0x28)
	printf("error \r\n");
	//write byte to eeprom
	TWIWrite(u8data);
	if (TWIGetStatus() != 0x28)
	printf("error \r\n");
	TWIStop();
	printf("success \r\n");
}


uint8_t EEReadByte(uint16_t u16addr, uint8_t *u8data)
{
	//uint8_t databyte;
	TWIStart();
	if (TWIGetStatus() != 0x08){
		uint8_t EEReadByte(uint16_t u16addr, uint8_t *u8data);
	}
	//uint8_t databyte;
	TWIStart();
	if (TWIGetStatus() != 0x08)
	printf("error \r\n");
	//select devise and send A2 A1 A0 address bits
	TWIWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7)));
	if (TWIGetStatus() != 0x18)
	printf("error \r\n");
	//send the rest of address
	TWIWrite((uint8_t)(u16addr));
	if (TWIGetStatus() != 0x28)
	printf("error \r\n");
	//send start
	TWIStart();
	if (TWIGetStatus() != 0x10)
	printf("error \r\n");
	//select devise and send read bit
	TWIWrite((EEDEVADR)|((uint8_t)((u16addr & 0x0700)>>7))|1);
	if (TWIGetStatus() != 0x40)
	printf("error \r\n");
	*u8data = TWIReadNACK();
	if (TWIGetStatus() != 0x58)
	printf("error \r\n");
	TWIStop();
	printf("success \r\n");
}

int main (void)
{
	USART_Init();
	usart_setup_stdio_stream();
	
	uint8_t u8ebyte;
	uint8_t u8erbyte;
	uint16_t u16eaddress = 0x07F0;
	uint8_t page = 5;
	uint8_t i;
	
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TWIInit();
	//assign our stream to standard I/O streams
	printf("\nWrite byte %#04x to eeprom address %#04x", 0x58, u16eaddress);
	EEWriteByte(u16eaddress, 0x58);
	printf("\nRead byte From eeprom");
	EEReadByte(u16eaddress, &u8ebyte);
	printf("\n*%#04x = %#04x", u16eaddress, u8ebyte);

}
