#include <avr/io.h> 
#include <avr/interrupt.h>


int OVF_CNT = 0; // Overflow Counter
int schaltsekunde = 0; // 8MHz/Prescaler(8)/TCTNT0(256) = 3906,25, jede vierte sekunde eine verzögerung zum ausgleichen

int main(void) { 
	
	DDRB = 0xFF; // B output LED
	PORTB = 0xFF;
	
	TCCR0 = (1<<CS01); // Timer0 configuration, Prescaler = 8 
	TIMSK |= (1<<TOIE0); // When the TOIE0 bit is written to one, and the I-bit in the Status Register is set (one), the Timer/Counter0 Overflow interrupt is enabled.
	sei();
	
	while(1) {
		
		} 
	}
	
	/* Der Overflow Interrupt Handler wird aufgerufen, wenn TCNT0 von 255 auf 0 wechselt */ 
	ISR(TIMER0_OVF_vect) {
		OVF_CNT++;
		
		if(schaltsekunde < 3 && OVF_CNT == 3906) {
			OVF_CNT = 0;
			schaltsekunde++;
			PORTB--;
		}
		
		else if  (schaltsekunde == 3 && OVF_CNT == 3907) {	
			OVF_CNT = 0;
			schaltsekunde = 0;
			PORTB--;
		}
	}