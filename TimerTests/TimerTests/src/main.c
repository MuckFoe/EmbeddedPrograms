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
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
/**
int counter = 0;


int out(void) {
	
	DDRB = 0xff;	//define PORTB as output pin for leds
	
	// 256/ 8 000 000=0,000032 1/0,000032=31250
	// 256/ 1 000 000=0,000256 1/0,000256=3906
	
	TCCR0 = (1<<CS01); // Timer0 configuration, Prescaler = 8
	TIMSK |= (1<<TOIE0); // enable Timer Overflow Interrupt
	sei(); // enable Global Interrupts aktivieren
	while(1)
	{
	}
}
/* Der Overflow Interrupt Handler wird aufgerufen, wenn TCNT0 von 255 auf 0 wechselt 
ISR(TIMER0_OVF_vect)
{
	if (counter == 3906){
		counter = 0;
		PORTB--;
		}else{
		counter++;
	}
}*/

