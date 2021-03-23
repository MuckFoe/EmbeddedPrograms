  /********************************************
  Author: Peter J. Vis
  Title: R2D2
  
  Microcontroller: ATmega32
  Crystal: 16 MHz
  Platform: Development System
  
  LIMITATIONS:
  No part of this work may be used in commercial
  applications without prior written permission.
  
  This work cannot be reproduced for blogging
  Copyright Protected. All Rights Reserved.
  
  PURPOSE:
  This program is used to test the Piezo
  speaker by generating musical notes.
  
  CIRCUIT:
  Piezo Speaker connected to PortC pin PC7.
  An 8 ? voice coil speaker could also be
  used with a 2 ? resistor in series.
  
  *********************************************/
  
  #define F_CPU 8000000UL
  
  #include <avr/io.h>
  #include <util/delay.h>
  
  #define SPEAKER_PORT PORTC
  #define SPEAKER_DDR DDRC
  #define SPEAKER_PIN 7
  
  void PLAYNOTE(float duration, float frequency);
  
  
  // function that wraps delay to delay a custom amount of time 
  // the amount of time is stored in a variable
  // ToDo: change to compute with modulo for maybe less cycles?
  void fakeDelay(uint16_t t) {
	  while(t > 0) {
		  _delay_ms(1);
		  t--;
	  }
	  return;
  }
  
  int main(void)
  {
	  PLAYNOTE(8000,20);
	  _delay_ms(2000);
	  PLAYNOTE(8000,50);
	  _delay_ms(2000);
	  PLAYNOTE(8000,100);
  }
  
  
  // ---------------------------------------
  // The PLAYNOTE function must be given the
  // duration, and frequency values.
  // The duration is how long the note
  // is played for. The frequency value
  // determines the musical note.
  // ---------------------------------------
  void PLAYNOTE(float duration, float frequency)
  {
	  // Physics variables
	  long int i,cycles;
	  volatile float half_period;
	  float wavelength;
	  
	  
	  // Standard physics formula.
	  wavelength=(1/frequency)*1000;
	  
	  // The number of cycles.
	  cycles=duration/wavelength;
	  
	  // The time between each toggle
	  half_period = wavelength/2;

	  // Data direction register Pin 7
	  // is set for output.
	  SPEAKER_DDR |= (1 << SPEAKER_PIN);
	  
	  // The output pin 7 is toggled
	  // for the 'cycles'number of times.
	  for (i=0;i<cycles;i++)
	  
	  {
		  fakeDelay(half_period); // Wait 1 half wavelength.
		  
		  SPEAKER_PORT |= (1 << SPEAKER_PIN); // Output 5 V to port Pin 7.
		  
		  fakeDelay(half_period); // Wait 1 half wavelength.
		  
		  SPEAKER_PORT &= ~(1 << SPEAKER_PIN); // 0 V at port pin 7.
		  
	  }
	  return; 
  }