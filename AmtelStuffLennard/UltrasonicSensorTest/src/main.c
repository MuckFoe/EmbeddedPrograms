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

#define  Trigger_pin	PA0	/* Trigger pin */
//Den Pinzustand kannst du im Register PINx abrufen - PIN eingang

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
	
	/* Als erstes stellst du das Datenrichtungsregister (Data-Direction-Register) kurz DDRx ein. 
	Dabei gibt das kleine x an, welchen Port (PORTA, PORTB, ...) du konfigurieren willst.
	Eine logische Null an der Stelle eines Pins ergibt für diesen Pin einen Eingang. 
	Eine logische Eins hingegen einen Ausgang.
	HIER UMGEKEHRT */
	DDRA = 0x01;		/* Make trigger pin as output */
	DDRB = 0xff;		/* Make DDRB all Ppins Output for LEDs */
	PORTD = 0xFF;		/* Turn on Pull-up - D is input,
	Ein Pullup-Widerstand verbindet unbenutzte Eingangspins (UND- und NAND-Gatter) mit der Gleichstromversorgungsspannung (Vcc), um den gegebenen Eingang HIGH zu halten.
	Ein Pulldown-Widerstand verbindet unbenutzte Eingangspins (ODER- und NOR-Gatter) mit Masse (0V), um den gegebenen Eingang LOW zu halten*/
	
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;		/* Set all bit to zero Normal operation */

	while(1)
	{
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		PORTA |= (1 << Trigger_pin);//set bit
		_delay_us(10);
		PORTA &= (~(1 << Trigger_pin));// clear bit
		
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x41;	/* Capture on rising edge, No prescaler*/
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */

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
		if(distance < 250){
			PORTB = ~((int)distance);
		}else{
			PORTB = 0x00;
		}
		_delay_ms(200);
	}
}
