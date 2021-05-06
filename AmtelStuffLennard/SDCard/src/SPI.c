#include <avr/io.h>
#include <stdio.h>
#include "SPI.h"

void initMaster( void )
{
	// MOSI, SCK and SS Output and MISO as Input
	DDRB |= (1 << MOSI) | (1 << SCK) | (1 << SS);

	// SPI Control Register: activate SPE (SPI enable) and set MSTR -> Master Mode (Master/Slave select) and set clock rate fosc/64
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);

	printf( "Init SPI Master complete\r\n" );
}

void SPITransmit( uint8_t data )
{
	SPDR = data;	// SPI Data Register

	while( !(SPSR & (1 << SPIF)) );	// data transfer complete SPI Interrupt Flag is set in SPI Status Register
}

uint8_t SPIReceive( void )
{
	while( !(SPSR & (1 << SPIF)) );	// whait till spif fires

	return SPDR; // return content of spi data register
}


void enableSlaveSelect( void )
{
	PORTB &= ~(1 << SS);
}

void disableSlaveSelect( void )
{
	PORTB |= (1 << SS);
}