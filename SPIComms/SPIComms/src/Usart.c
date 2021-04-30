/*
 * Usart.c
 *
 * Created: 30/04/2021 15:19:23
 *  Author: Moe
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "Usart.h"

// Global Variables for printf
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

void USART_Init() {
	UBRRH = (unsigned char) ( UBBR_BAUD >> 8 );			// Set Baudrate for High Register
	UBRRL = (unsigned char) UBBR_BAUD;					// Set Baudrate for Low Register
	UCSRB = (1 << RXEN) | (1 << TXEN) ;	// Enable Rx and Tx and Interrupt for Rx
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);	// USREL = 1 to write to URSRC, Set frame 8-Bit Charakter-Size
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
void usart_putchar(unsigned char data) {
	while (!(UCSRA & (1<<UDRE))) /*Wait for empty transmit buffer */
	;
	UDR = data;                  /*Put data into buffer, sends the data */
}


//This function is called by printf as a stream handler
int usart_putchar_printf(char var, FILE *stream) {
	usart_putchar(var);
	return 0;
}
/*
 *	Setup stdio stream.
 */
void usart_setup_stdio_stream( void )
{
	stdout = &mystdout; 
}