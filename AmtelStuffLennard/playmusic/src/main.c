/********************************************

2: Author: Peter J. Vis

3: Title: R2D2

4:

5: Microcontroller: ATmega32

6: Crystal: 16 MHz

7: Platform: Development System

8:

9: LIMITATIONS:

10: No psrt of this work may be used in commercial

11: applications without prior written permission.

12:

13: This work cannot be reproduced for blogging

14: Copyright Protected. All Rights Reserved.

15:

16: PURPOSE:

17: This program is used to test the Piezo

18: speaker by generating musical notes.

19:

20: CIRCUIT:

21: Piezo Speaker connected to PortC pin PC7.
READ  Audio Tone Generator using AVR Microcontroller

22: An 8 ? voice coil speaker could also be

23: used with a 2 ? resistor in series.

24:

65: *********************************************/



#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include "notes.h"
#include <util/delay.h>


#define SPEAKER_PORT PORTD
#define SPEAKER_DDR DDRD
	/*
   PD7 = OC2
   PD6 = ICP
   PD5 = OC1A
   PD4 = OC1B
   PD3 = INT1
   PD2 = INT0
   PD1 = TXD
   PD0 = RXD
 */
#define SPEAKER_PIN 4

#define OCTAVEPIN 1
#define ELISEPIN 2
#define STARWARSPIN 3
#define JUDEPIN 4

void init()
{
	//Set DDRB as INPUT
	DDRB = 0x00;
	//DDRB = (1 << OCTAVEPIN) | (1 << ELISEPIN) | (1 << STARWARSPIN) | (1 << JUDEPIN);
	
	// Data direction register
	// is set for output.
	SPEAKER_DDR |= (1 << SPEAKER_PIN);
		
	//Timer Counter Control Register 1A auf COM1B1 ( Löschen von OC1A/OC1B bei Compare Match (Low-Pegel) )
	TCCR1A |= (1<< COM1B1);
	
	//mode 8, PWM, Phase and Frequency Correct (TOP value is ICR1) WGM = waveform generation mode und CS11 prescaler(8)
	TCCR1B |= (1<< WGM13) 	|  ( 1 << CS11); 		
}

/**
	Plays music.
*/

void PlayMusic( const int* pMusicNotes /** Pointer to table containing music data */,
				uint8_t tempo /** paying tempo from 0 to 100. Higher value = slower playback*/ )
{
	int duration;
	int note;
	uint16_t delay = tempo * 1000;

	while( *pMusicNotes )
	{
		note = *pMusicNotes;
		pMusicNotes++;

		duration = *pMusicNotes;
		pMusicNotes++;

		if( p == note )
		{
			//pause, do not generate any sound
			OCR1B = 0; //Speaker Pin = PD4 = 0CR1B
		}
		else
		{
			//not a pause, generate tone
			OCR1B = 100;

			//set frequency
			//"Wenn am Input Capture Pin ICP die gemäß Einstellungen im TCCR1B
			//definierte Flanke erkannt wird, so wird der aktuelle Inhalt des
			//Datenregisters TCNT1H/TCNT1L sofort in dieses Register (ICR1H und ICR1L)
			//kopiert und das Input Capture Flag ICF1 im Timer Interrupt Flag Register
			//TIFR gesetzt."
			ICR1H = (note >> 8);
			ICR1L = note;
		}

		//wait duration
		for(int i=0;i<32-duration;i++)
		{
			_delay_loop_2( delay );
		}

	}
	

	//turn off any sound
	OCR1B = 0;
}


int main(void)
{
		init();

		while(1)
		{
			if(PINB & OCTAVEPIN) PlayMusic( octave, 40 );
			else if(PINB & ELISEPIN) PlayMusic( furelise, 20 );
			else if(PINB & STARWARSPIN) PlayMusic( starwars, 17 );
			else if(PINB & JUDEPIN) PlayMusic( Jude, 35 );
			else break;
		}
		
	return 0;
}
