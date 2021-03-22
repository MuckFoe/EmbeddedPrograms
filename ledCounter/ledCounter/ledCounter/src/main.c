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
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

int decimalToBinary(int decimalnum)
{
	long binarynum = 0;
	int rem, temp = 1;

	while (decimalnum!=0)
	{
		rem = decimalnum%2;
		if(rem == 1){
			rem = 0;
			}else{
			rem =1 ;
		}
		decimalnum = decimalnum / 2;
		binarynum = binarynum + rem*temp;
		temp = temp * 10;
	}
	return binarynum;
}


void main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	DDRD = 0x00;
	DDRB = 0xff;
	
	PORTB = 0xff;
	/* Insert application code here, after the board has been initialized. */
	while(1){
		/**
		if (PIND != 0xff){
			//cnt += 1;
			//printf(decimalToBinary(cnt));
			_delay_ms(800);
			PORTB--;
		}
		
		//PORTB = decimalToBinary(cnt);
		*/
		DDRA = 0x01;
	}
}

