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
  
  // My Prototype is simply called PLAYNOTE.
  void PLAYNOTE(float duration, float frequency);
  
  
void fakeDelay(uint16_t t) {
	while(t > 0) {
		_delay_ms(1);
		t--;
	}
	return;
}
  
  int main(void)
  {
	  
	  PLAYNOTE(8000,200);  // Musical note 880 Hz
	  _delay_ms(1000);
	  PLAYNOTE(8000,932);
	  _delay_ms(1000);
	  PLAYNOTE(8000,2000);
	  _delay_ms(1000);
	  PLAYNOTE(400,1047);
	  PLAYNOTE(400,1109);
	  PLAYNOTE(400,1175);
	  PLAYNOTE(400,1244);
	  PLAYNOTE(400,1319);
	  PLAYNOTE(400,1397);
	  PLAYNOTE(400,1480);
	  PLAYNOTE(400,1568);
	  _delay_ms(1000);
	  PLAYNOTE(400,1660);  // Musical note 1660 Hz
	  
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
	  
	  wavelength=(1/frequency)*1000;
	  // Standard physics formula.
	  cycles=duration/wavelength;
	  // The number of cycles.
	  half_period = wavelength/2;
	  // The time between each toggle.
	  
	  // Data direction register Pin 7
	  // is set for output.
	  SPEAKER_DDR |= (1 << SPEAKER_PIN);
	  
	  for (i=0;i<cycles;i++)
	  // The output pin 7 is toggled
	  // for the 'cycles'number of times.
	  // --------------------------------
	  
	  {
		  fakeDelay(half_period);
		  // Wait 1 half wavelength.
		  SPEAKER_PORT |= (1 << SPEAKER_PIN);
		  // Output 5 V to port Pin 7.
		  fakeDelay(half_period);
		  // Wait 1 half wavelength.
		  SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
		  // 0 V at port pin 7.
	  }
	  
	  return;
	  // Return to main()
	  
  }