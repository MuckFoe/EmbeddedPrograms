#ifndef USART_H_
#define USART_H_

#include <stdio.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#define BAUD		9600UL
#define UBBR_BAUD	(F_CPU/(BAUD*16)-1)


void USART_Init( void );

void USART_Transmit( unsigned char data );

int usart_transmit_printf( char var, FILE *stream );

uint8_t USART_Receive( void );

void usart_putchar(unsigned char );

int usart_putchar_printf(char , FILE *);

void usart_setup_stdio_stream( void );



#endif