
/******************************************** 
Date: SS21 
Press switch and corresponding LED will blink 
STK500, ATMega 32, 8 MHZ, LED on Port C, Switches on Port D 
*********************************************/ 
#include <avr/io.h> //includes MCU specific *.h files
#include <util/delay.h>

#define LED0 PORTC = 0b11111110;
#define LED1 PORTC = 0b11111101;
#define LED2 PORTC = 0b11111011;
#define LED3 PORTC = 0b11110111;
#define LED4 PORTC = 0b11101111;
#define LED5 PORTC = 0b11011111;
#define LED6 PORTC = 0b10111111;
#define LED7 PORTC = 0b01111111;

void countUp() {
	
		DDRD = 0x00; //Port D Input
		DDRC = 0xff; //Port C output
		
		PORTC = 0xff;
		
		while (1) {
			_delay_ms(10000);
			PORTC--;
		}
}

void showNumbersBinaryPlz(){
	 while(1){
		 		DDRD = 0x00; //Port D Input
		 		DDRC = 0xff; //Port C output
				 		while (1) {
					 		PORTC = 0xff;
					 		_delay_ms(5000);
					 		LED0;
					 		_delay_ms(5000);
					 		LED1;
					 		_delay_ms(5000);
					 		LED2;
					 		_delay_ms(5000);
					 		LED3;
					 		_delay_ms(5000);
					 		LED4;
					 		_delay_ms(5000);
					 		LED5;
					 		_delay_ms(5000);
					 		LED6;
					 		_delay_ms(5000);
					 		LED7;
					 		_delay_ms(5000);
					 		PORTC = ~0b00000001;
					 		_delay_ms(5000);
				 		}
		 
	 }
}


int main(void) {
showNumbersBinaryPlz();
return 0;
}