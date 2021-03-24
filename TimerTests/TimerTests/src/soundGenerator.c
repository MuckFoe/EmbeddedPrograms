/*
ATmega32 @ 8MHz
Plays an 8bit/8000 sample PCM audio on OC1A output
*/

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
//#include "pcm_sample.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#define SAMPLE_RATE 8000;
#define DEFAULT_VOLUME 100

#include "notes.h"


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
	DDRD = (1<<PD5);


	// WGM10 = PWM, TOP = 0x00FF(255), update of ocr1x at TOP, TOV1 flag set on Bottom
	// COM1A1 Compare Output mode for compare unit A -> COM1A1  clear OC1A on compare match
	TCCR1A = (1 << COM1A1) | (1 << WGM10);
	
	
	// Fast PWM, 8bit
	// Prescaler: clk/1 = 8MHz
	// PWM frequency = 8MHz / (255 + 1) = 31.25kHz
	// WGM12 = CTC mode, TOP is OCR1A, Updates OCR1B immediately, TOV1 flag set on MAX
	// CS10 = clk 1, no prescaler
	// CS11 = 8 prescaler
	TCCR1B = (1 << WGM12) | (1 << CS10);
	
	// set initial duty cycle to zero
	OCR1A = 0;
	
	
	// Setup Timer0
	TCCR0|=(1<<CS00);	// clk normal no prescaling 
	TCNT0=0;			// reset timer to 0
	TIMSK|=(1<<TOIE0);	// enable overflow interrupt 
	
	// we'll need to find out what this does
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
	pwm_init();
	while(1){
		PlayMusic( starwars, 17 );
	}
		;//do nothing
}
*/