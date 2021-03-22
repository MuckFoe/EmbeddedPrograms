/*
 * Ultrasonic sensor HC-05 interfacing with AVR ATmega16
 * http://www.electronicwings.com
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#define Trigger_pin0	PA0	/* Trigger pin zero*/
#define Trigger_pin1	PA1 // Trigger pin one

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}

int main(void)
{
	char string[10];
	long count;
	double distance;
	
	DDRA = 0x03;		/* Make trigger pinS as output */
	DDRB = 0xFF;
	PORTB = 0xFF;
	PORTD = 0xFF;		/* Turn on Pull-up */
	
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	// Enable Timer/Counter1 overflow interrupts  the corresponding interrupt vector is executed when TOV1 flag in TIFR is set
	TCCR1A = 0;		// Set all bit to zero Normal operation
	TCCR2 = 0;

	while(1)
	{
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		PORTA |= (1 << Trigger_pin0) | (1 << Trigger_pin1) ;// & (1 << Trigger_pin1); // setting the trigger PIN0 and trigger PIN1 to 1
		_delay_us(10);
		PORTA &= (~(1 << Trigger_pin0)) | (~(1 << Trigger_pin1));// & (~(1<<Trigger_pin1)); // setting the trigger PIN0 and trigger PIN1 to 0
		
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x41;	// Capture on rising edge, No prescaler (0b 0100 0001) (bit 0-2 prescaler selection) 
						// (bit 3-4 Timer1 Mode Selection aka normal, pwm, ctc)(bit 6 input captuer edge aka 0 on falling 1 on rising)
						// (bit 7 Input capture noise canceller = causes delay of 4 clock cycles (change persists longer than 4 cycles))
		TIFR = 1<<ICF1;	// Input Capture Flag in TIFR, flag is set on event on ICP1 Pin (is cleared by writing a 1 to its bit position)
		TIFR = 1<<TOV1;	// Clear Timer Overflow flag, TOV1 is set when timer overflows (is cleared by writing a 1 to its bit position)
		// TIFR TimerInterruptFlagRegister
		// TIMSK TimerInterruptMaskRegister
		/*Calculate width of Echo by Input Capture (ICP) */
		
		while ((TIFR & (1 << ICF1)) == 0);/* Wait for rising edge */
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
		TimerOverflow = 0;/* Clear Timer overflow count */

		while ((TIFR & (1 << ICF1)) == 0);/* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow);	/* Take count */
		/* 8MHz Timer freq, sound speed =343 m/s */
		distance = (double)count / 466.47;

		dtostrf(distance, 2, 2, string);/* distance to string */
		if(distance < 125){
			PORTB = distance;
		}
		strcat(string, " cm   ");	/* Concat unit i.e.cm */
		_delay_ms(50);
	}
}