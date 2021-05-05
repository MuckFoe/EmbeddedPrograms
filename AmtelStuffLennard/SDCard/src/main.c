#include <avr/io.h>
#include <util/delay.h>
#include "SD.h"
#include "SPI.h"

// https://mansfield-devine.com/speculatrix/2018/01/avr-basics-spi-on-the-atmega-part-2/

void init() {
	
	DDRC = 0xFF; // C Output LED
	PORTC = 0xFF;
	
	initMaster();
	initSD();
}

int main (void) { 
	
	init();
	
	_delay_ms(100); //just to be sure
	
	SDWriteSingleBlock(8);
	
	_delay_ms(100); //just to be sure
	
	uint8_t buffer; // dunno what im doing
	while(1) PORTC = SDReadSingleBlock(buffer); //show those
		
return 0; 
}