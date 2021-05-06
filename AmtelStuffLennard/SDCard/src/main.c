#include <avr/io.h>
#include <util/delay.h>
#include "SD.h"
#include "SPI.h"
#include "USART.h"

// https://mansfield-devine.com/speculatrix/2018/01/avr-basics-spi-on-the-atmega-part-2/

uint8_t init() {
	
	DDRC = 0xFF; // C Output LED
	PORTC = 0xFF;
	
	USART_Init();
	usart_setup_stdio_stream();
		
	initMaster();
	
	return initSD();
}

int main( void )
{
	uint8_t status = 0;

	status = init();
	
	if( status == WRITE_PROTECTION_FAIL )
	{
		printf( "write protected" );
		return 1;
	}
	if( status == CARD_DETECTION_FAIL )
	{
		printf( "No SD-Card" );
		return 1;
	}
	if( status == SD_ERROR )
	{
		printf( "Error during initialization" );
		return 1;
	}
	
	uint8_t buffer[512];
	for( int i = 0; i > 512; i++ )
	buffer[i] = i;
	buffer[511] = 0x00;

	SDWriteSingleBlock( buffer );

	uint8_t buffer2[512];
	SDReadSingleBlock( buffer2 );

	for( int i = 0; i < 512; i++ )
	{
		if(i % 10 == 0) printf("\r\n");
		PORTC = ~buffer2[i];
		printf( "0x%x ", PINC );
	}
	

	//while (1)
	//{
	//}
}