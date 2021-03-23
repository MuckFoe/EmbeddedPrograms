/*
ATmega32 @ 8MHz
Plays an 8bit/8000 sample PCM audio on OC1A output
*/

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
//#include "pcm_sample.h"
#include <avr/interrupt.h>
#define SAMPLE_RATE 8000;


volatile uint16_t sample;
int sample_count;

const long pcm_length=400;

const unsigned char pcm_samples[] PROGMEM ={
	116,97,144,26,0,0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128
};

// initialise the PWM
void pwm_init(void)
{
	// use OC1A pin as output
	DDRD = (1 << PD5);

	
	// clear OC1A on compare match
	// set OC1A at BOTTOM, non-inverting mode
	// Fast PWM, 8bit
	// Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when downcounting.
	// WGM10 = PWM, Phase Correct, 8-bit 0x00FF(255) TOP(update of ocr1x) BOTTOM (TOV1 flag set on)
	TCCR1A = (1 << COM1A1) | (1 << WGM10);
	
	
	// Fast PWM, 8bit
	// Prescaler: clk/1 = 8MHz
	// PWM frequency = 8MHz / (255 + 1) = 31.25kHz
	TCCR1B = (1 << WGM12) | (1 << CS10);
	
	// set initial duty cycle to zero
	OCR1A = 0;
	
	// Setup Timer0
	
	TCCR0|=(1<<CS00);
	TCNT0=0;
	TIMSK|=(1<<TOIE0);
	sample_count = 4;
	sei(); //Enable interrupts
}


/**

ISR(TIMER0_OVF_vect)
{

	sample_count--;
	if (sample_count == 0)
	{
		sample_count = 4;
		OCR1A = pgm_read_byte(&pcm_samples[sample++]);
		if(sample>pcm_length)sample=0;
	}
}




int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	//pwm_init();
	while(1);//do nothing
}
*/