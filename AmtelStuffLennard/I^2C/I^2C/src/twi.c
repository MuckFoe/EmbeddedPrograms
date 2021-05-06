/*
* twi.c
*
* Created: 05/05/2021 17:43:30
*  Author: Moe
*/
#include "twi.h"
#include <stdio.h>
#include <avr/io.h>


void TWIInit(void)
{
	//set SCL to 400kHz
	TWSR = 0x00; //TWI Status Register
	TWBR = 0x0C; // TWI Bit Rate Register
	//enable TWI
	TWCR = (1<<TWEN); //TWR Control Register - TWEN ->TWI Freigeben
}
//send start signal
void TWIStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //TWINT = Setzen: Starten, TWSTA = start ausführen, TWEN = TWI freigeben
	while ((TWCR & (1<<TWINT)) == 0); //solange noch gestartet wird warten bis gesetzt TWINT = / Anzeigen: fertig
}

//send stop signal
void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); // TWINT = Setzen: Starten/ Anzeigen: fertig, TWSTO = STopp ausführen, TWEN = TWI freigeben
}

void TWIWrite(uint8_t u8data)
{
	TWDR = u8data; //TWI Data Register
	TWCR = (1<<TWINT)|(1<<TWEN); // TWINT = Setzen: Starten/ Anzeigen: fertig, TWEN = TWI freigeben
	while ((TWCR & (1<<TWINT)) == 0); //solange noch gestartet wird warten bis gesetzt TWINT = / Anzeigen: fertig
}
//read byte with ACK
uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); // TWINT = Setzen: Starten/ Anzeigen: fertig, TWEN = TWI freigeben, TWEA = ACK ausführen
	while ((TWCR & (1<<TWINT)) == 0); //solange noch gestartet wird warten bis gesetzt TWINT = / Anzeigen: fertig
	return TWDR; // gib zurück was angekommen ist
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
	status = TWSR & TWI_NO_STATE; // TWI STatus Register, TWI_NO_STATE = 0xF8 No relevant state information available
	return status;
}
