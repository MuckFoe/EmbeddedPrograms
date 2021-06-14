/*
   Bluetooth_Interface with ATmega16 to Control LED via smartphone
   http://www.electronicwings.com
 
 */ 

#include <avr/io.h>
#include "Usart.c"	/* include USART library */

#define LED PORTB		/* connected LED on PORT pin */

int main(void)
{
	char Data_in;
	DDRB = 0xff;		/* make PORT as output port */
	USART_Init();	/* initialize USART with 9600 baud rate */
	LED = 1; /* LED off */
	
	while(1)
	{
		Data_in = USART_Receive();	/* receive data from Bluetooth device*/
		
		if(Data_in == 1 || Data_in == '1')
		{
			LED |= (1<<PB0);	/* Turn ON LED */
			USART_putstring("LED_ON");/* send status of LED i.e. LED ON */
			
		}
		else if(Data_in =='2' || Data_in == 2)
		{
			LED &= ~(1<<PB0);	/* Turn OFF LED */
			USART_putstring("LED_OFF"); /* send status of LED i.e. LED OFF */
		}
		else
			USART_putstring("Select proper option"); /* send message for selecting proper option */
	}		
}