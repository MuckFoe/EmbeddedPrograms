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
#include <util/delay.h>
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 8.000.000UL //Check -> Fuses -> SUT_CKSEL…
#endif
#define UART_BAUD_RATE 9600

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 8UL))) - 1)


void USART_Init(unsigned int baud) {
	UBRRH = (unsigned char)(baud>>8);

	UBRRL = (unsigned char)baud; //set baud rate
	UCSRB = (1<<RXEN)|(1<<TXEN);
	
	
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // Enable rx and tx
	
	// Set frame format: 8data, 1stop bit, parity none (initial value)
}

/* Initialize USART */
void USART_init(void){
	UBRRH = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRRL = (uint8_t)(BAUD_PRESCALLER);
	UCSRB = (1<<RXEN)|(1<<TXEN);
	UCSRC = (1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL);
}

// URSEL=1: USCRC, URSEL = 0: UBRRH (initial value)
void USART_Transmit(uint8_t data) {
	
	// Wait for empty transmit buffer
	while( !( UCSRA & (1<<UDRE)) ) ;
	
	
	UDR = data;// Put data into buffer, sends the data
}
uint8_t USART_Receive(void) {
	
	while( !(UCSRA & (1<<RXC)) );// Wait for data to be received
	
	
	return UDR;// Get and return received data from buffer
}

int main (void)
{
	unsigned char dataReceived;
	DDRB = 0xFF;
	PORTB = 0xFF;

	USART_init();
	
	
	//dataReceived =USART_Receive();
	
	//PORTB = 0x00;
	while(1){
		USART_Transmit(120);
		PORTB = 0x00;
		_delay_ms(1000);
		PORTB = 0xFF;
	}
	PORTB = 0xFF;
}
