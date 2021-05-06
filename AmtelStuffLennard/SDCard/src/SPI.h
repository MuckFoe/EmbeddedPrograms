#ifndef __SPI_H_
#define __SPI_H_
#include <avr/io.h>


#define SS		DDB4
#define MOSI	DDB5
#define MISO	DDB6
#define SCK		DDB7

/**
*	Initialize SPI Master
*/
void initMaster( void );

/**
*	Write data to SPI Data Register and transmit.
*/
void SPITransmit( uint8_t data );

/**
*	Receive bytes.
*/
uint8_t SPIReceive( void );

/**
*	SS is negated. To activate the pin it must be deleted.
*/
void enableSlaveSelect( void );

/**
*	SS is negated. To disable the pin it must be set.
*/
void disableSlaveSelect( void );

#endif