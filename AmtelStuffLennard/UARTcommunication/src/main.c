/*
 * Example: USART_AVR
 * Target: ATmega32/ATmega32A
 * Frequency: 8 MHz Fuse: LFUSE=0xFF HFUSE=0x99
 * NOTE: Make sure to burn right fuse bits after loading USART_AVR.hex file to MCU. 
 * Remember Tx of FT232RL Adapter is connected to RXD and Rx connected to TXD of ATmega32A. 
 *
*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define BAUDRATE 9600
#define BAUDDINGS 52
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);



int main(void){
  USART_init();                     // Call the USART initialization code
  
  while(1){
//	  PORTB = 0xFF;           
	  USART_putstring("hallo");
//    PORTB = USART_receive(); // Pass the string to the USART_putstring function and sends it over the serial
    _delay_ms(2000);       
  }
  
  return 0;
}
/* Initialize USART */
void USART_init(void){
	DDRB = 0xFF;
	PORTB = 0xFF;
  UBRRH = (uint8_t)(BAUDDINGS>>8);
  UBRRL = (uint8_t)(BAUDDINGS);
  UCSRB = (1<<RXEN)|(1<<TXEN);
  UCSRC = (1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL);
}
/* Function to receive byte/char */
unsigned char USART_receive(void){
  while(!(UCSRA & (1<<RXC)));
  return UDR;
}
/* Function to send byte/char */
void USART_send( unsigned char data){
  while(!(UCSRA & (1<<UDRE)));
  UDR = data;
}
/* Send string */
void USART_putstring(char* StringPtr){
  while(*StringPtr != 0x00){
    USART_send(*StringPtr);
  StringPtr++;}
  
}