#include <asf.h>
#include <avr/io.h>
#include <Spisd.h>
#include <util/delay.h>
#include <Usart.h>
#include "SdC.h"
#define F_CPU 8000000 ML

/*
MISO = PB6
MOSI = PB5
SCK = PB7
SS = PB4
*/

void SPI_MasterInit(void) {
	//*Set MOSI, SCK, SS output 
	/* Enable SPI, Master, set clock rate fosc/64 */ 
	//SPCR SPI Steuerregister
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); //SPE = SPI frei MSTR =Master SPR1=Systemtakt/64MhZ
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);  //DDB5 Mosi, DDB7 SCK output, DDB4 SS output
}

void SPI_MasterTransmit(char cData) { 
	SPDR = cData; // SPI Data register
	while(!(SPSR & (1<<SPIF))){} //Wait for transmission complete ;
}
void SPI_Reset(){
	SPCR = 0x00;
	DDRB = 0x00;
}

void SPI_SlaveInit(void) { 
	DDRB = (1<<DDB6); //* Set MISO output /* Enable SPI */ 
	SPCR = (1<<SPE); 
}

char SPI_SlaveReceive(void) { 
	while(!(SPSR & (1<<SPIF))) //* Wait for reception complete ;
	return SPDR; //* Return Data Register 
}


/*
void reset_sdcard(void){
	DDRB = (1<<EN1);
	PORTB = (1<<EN1);
	_delay_ms(50);
	PORTB = (0<<EN1);
}*/

void power_on_sdcard(void){
	DDRB = (1<<EN2);
	PORTB = (1<<EN2);
	_delay_ms(50);
}

void reset_sdcard(){
	DDRB = (1<<EN1);
	DDRB = (1<<EN2);
	PORTB = (1<<EN1);
	PORTB &= ~(1<<EN2);
	_delay_ms(50);
	PORTB &= ~(1<<EN1); 
	PORTB = (1<<EN2);
}

void sanity(){
	DDRA = (1<<LED);
	PORTA = (1<<LED);
	
	_delay_ms(2000);
	PORTA &= ~(1<<LED); 
}
//Send a command to the SD card
//Input: uint8_t cmd command
//      uint32_t arg command parameters
//      uint8_t CRC CRC CRC check value
//Return value: the response returned by SD card
uint8_t SdSendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	
	SPI_MasterTransmit(cmd | 0x40);//Write commands separately
	SPI_MasterTransmit(arg >> 24);
	SPI_MasterTransmit(arg >> 16);
	SPI_MasterTransmit(arg >> 8);
	SPI_MasterTransmit(arg);
	SPI_MasterTransmit(crc);

	return SPI_SlaveReceive();
}


int main (void)
{
	USART_Init();
	usart_setup_stdio_stream();
	DDRA = (1<<LED);
	DDRC = 0xFF;
	//DDRB = (1<<DDB0);

	PORTA = 0x00;
	PORTC = 0x00;

	/*
	printf("InitMaster \r\n");
	SPI_MasterInit();

	power_on_sdcard();
	//reset_sdcard();
	//sanity();
	printf("StartingTransmition \r\n");
	
	printf(SdSendCmd(cmd0, 0x0000, 0x95));
	printf("\r\n");
	
	printf(SdSendCmd(cmd1, 0x0000, 0x95));
	printf("\r\n");
	printf(SdSendCmd(cmd0, 0x0000, 0x95));
	printf("\r\n");
	*/
	spi_init_master();
	printf( "Test= 0x%02x \n ", sd_init() );
}


