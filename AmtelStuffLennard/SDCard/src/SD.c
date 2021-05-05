#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "SD.h"


volatile uint16_t timer = 0;

unsigned char CMD0[] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x95 };		// GO_IDLE_STATE
unsigned char CMD55[] = { 0x77, 0x00, 0x00, 0x00, 0x00, 0x01 };		// APP_CMD
unsigned char ACMD41[] = { 0x69, 0x00, 0x00, 0x00, 0x00, 0x01 };	// SD_SEND_OP_COND
unsigned char CMD16[] = { 0x50, 0x00, 0x00, 0x02, 0x00, 0x01 };		// SET_BLOCKLEN (Blocksize 512 Bytes) Block length of SD-Card can be set with CMD16 (SET_BLOCKLEN. For SDHC and SDXC cards default block length is set to 512 bytes.
unsigned char CMD17[] = { 0x51, 0x00, 0x00, 0x08, 0x00, 0x01 };		// READ_SINGLE_BLOCK
unsigned char CMD24[] = { 0x58, 0x00, 0x00, 0x08, 0x00, 0x01 };		// WRITE_SINGLE_BLOCK
unsigned char CMD8[] = { 0x48, 0x00, 0x00, 0x01, 0xaa, 0x87 };		// SEND_IF_COND (interface condition)
unsigned char CMD58[] = { 0x7a, 0x00, 0x00, 0x00, 0x00, 0x01 };		// READ_OCR
unsigned char CMD13[] = { 0x4d, 0x00, 0x00, 0x00, 0x00, 0x01 };		// SEND_STATUS
unsigned char ACMD22[] = { 0x56, 0x00, 0x00, 0x00, 0x00, 0x01 };	// SEND_NUM_WR_BLOCKS



uint8_t initSD( void )
{
	uint8_t result[5], terminate = 0; //counter until termination
	
	printf( "Start SD init\r\n" );

	initTimer0();			// call timer init

	printf( "Start SD Holder init and reset \r\n" );
	
	initSDHolder();	// Init holder
	resetSD();			// hardware reset holder
	setDelay_ms( 250 );	// Chapter 6.4. Power Scheme -> sd-card vdd_min is reached within 250ms

	printf( "Start Power Up Sequence \r\n" );
	
	SDPowerUp();		// Power-Up sequence to communicate with sd
	
	if( isWriteProtected() )
		return WRITE_PROTECTION_FAIL;
	else printf("not write protected\r\n");	
	
	if( isNoCardDetected() )
		return CARD_DETECTION_FAIL;
	else printf("Card detected\r\n");

	// bring the SD-Card to idle state.
	// Chapter 6.4. Power Scheme -> In case of SPI host, CMD0 shall be the first command to send the card to SPI mode.
	while( (result[0] = SD_CMD_with_r1_Response( CMD0 )) != SD_IDLE_STATE )		// CMD0 --> Answer is a R1-Response [SD Spec Chapter 7.3.2.1 Format R1]
	{
		printf( "Answer CMD0: 0x%02x \n ", result[0] );
		terminate++;
		if( terminate > 20 ) return SD_ERROR;	// failed
	}
	printf( "Result CMD0 (GO_IDLE_STATE): 0x%02x \n\n ", result[0] );

	// Send Interface Condition Command -> Card has to be in idle state -> checks voltage
	// SD Spec Chapter 4.3.13 Send Interface Condition Command (CMD8)
	SDSendCMD8( result );			// CMD8 interface condition

	if( result[0] != SD_IDLE_STATE )		// answer other than IDLE_STATE leads to an error
		return SD_ERROR;

	if( result[4] != SD_R7_CHECK_PATTERN )	// checks if the echo pattern is the same as what was sent
		return SD_ERROR;

	terminate = 0;
	do 
	{
		if( terminate >= 100 ) return SD_ERROR;

		// This command, when received by the card, causes the card to interpret the following command as an application-specific command, ACMD.
		// If you send the card a CMD55 (this includes waiting for the response) and then another command, it will be interpreted as an "application specific" command
		// SD Spec Chapter 4.3.9.1 Application-Specific-Command - APP_CMD (CMD55)
		result[0] = SD_CMD_with_r1_Response( CMD55 );		// CMD55

		// SD_SEND_OP_COND (ACMD41) is used to start initialization and to check if the card has completed
		// initialization. It is mandatory to issue CMD8 prior to the first ACMD41
		// SD Spec Chapter 7.2.1 Mode Selection and Initialization and 4.2.3.1 Initialization Command (ACMD41)
		if( result[0] < SD_ERASE_RESET )	// 0x01 means idle and 0x00 means init completed, therefore is all greater than 1 an error state
			result[0] = SD_CMD_with_r1_Response( ACMD41 );	// ACMD41

		setDelay_ms( 10 );	// waiting 
		terminate++;
	} while ( result[0] != SD_INIT_COMPLETED );	// 0x01 means card is still initializing (idle) 0x00 means completion of initialization.

	// Read from OCR Register (operation conditions register)
	SDSendCMD58( result );	// CMD58 read OCR (Output Compare Register)


	printf("Init SD successfull \n");

	return SD_RUNNING;
}


void initSDHolder( void )
{
	DDRB |= (1 << EN1) | (1 << EN2);	// EN1 = Reset SD-Card; EN2 = Power SD-Card Holder
	DDRA |= (1 << LED);
	PORTA &= ~(1 << LED);
}


void resetSD( void )
{
	PORTB |= (1 << EN1);
	PORTB &= ~(1 << EN2);
	setDelay_ms( 75 );
	PORTB &= ~(1 << EN1);
	PORTB |= (1 << EN2);
}


void SDPowerUp( void )
{
	disableSlaveSelect();			// make sure card is deselected (High signal)
	setDelay_ms( 10 );					// time for SD-Card to power up
	SPITransmit( 0xff );
	disableSlaveSelect();			// deselect SD-Card (High signal)
	for( uint8_t i = 0; i < 80; i++ )	// send 80 clock cycles to synchronize (8Mhz --> 0,008ms)
		SPITransmit( 0xff );	 
}


void initTimer0( void )
{
	TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00);	// CTC Mode (Clear timer on Compare); Clock Select -> Prescaler 64
	OCR0 = 125-1;										// Output Compare Register 0CR0 = ((8000000 Hz/64)/1000 ms) = 125 Steps per Second
	TIMSK = (1 << OCIE0);								// Timer Interrupt Mask Register: activate Compare Interrupt
	sei();												// activate Global Interrupts
}



void SDSendCMD( uint8_t *cmd )
{	
	printf( "Send Command \n" );

	for( uint8_t i = 0; i < 6; ++i )
		SPITransmit( cmd[i] );	
}



uint8_t SDRead_r1( void )
{
	printf( "Start reading R1 result \n" );

	uint8_t r1_result, terminate_counter = 0;

	do {
		SPITransmit( 0xff );
		r1_result = SPIReceive();		// run loop till actual data is received
		printf( "---> R1-Result = 0x%02x \n", r1_result );

		terminate_counter++;
	} while( (r1_result == 0xff) && (terminate_counter < 8) );	// abort loop if no response is received for 8 bytes or the result is not equal to 0xff

	return r1_result;
}


void SDRead_r3( uint8_t *result )
{
	printf( "R3-Result is called \n" );

	SDRead_r7( result );	// forward to r7-response method
}


void SDRead_r7( uint8_t *result )
{
	printf( "Start reading R7 result \n" );

	result[0] = SDRead_r1();	// 1 byte R1 response token
	printf( "---> R7-Result[0] = 0x%02x \n", result[0] );

	if( result[0] > 1 ) return;		// if R1-Response has an error

	// run loop till the last 4 bytes of R7-Response are received.
	for( uint8_t i = 1; i < 5; ++i )
	{
		SPITransmit( 0xff );
		result[i] = SPIReceive();		
		printf( "---> R7-Result[%i] = 0x%02x \n", i, result[i] );
	}
}


uint8_t SD_CMD_with_r1_Response( uint8_t *cmd )
{
	printf( "Start sending command with r1 response \n" );

	// Enable Chip Select (CS)
	SPITransmit( 0xff );
	enableSlaveSelect();
	SPITransmit( 0xff );

	SDSendCMD( cmd );	// send the parameter that was passed

	uint8_t r1_result = SDRead_r1();	// collect R1-Reponse from cmd

	// Disable Chip Select (CS)
	SPITransmit( 0xff );
	disableSlaveSelect();
	SPITransmit( 0xff );

	return r1_result;
}


void SDSendCMD8( uint8_t *result )
{
	printf( "Start sending if_cond (CMD8) \n" );

	// Enable Chip Select (CS)
	SPITransmit( 0xff );
	enableSlaveSelect();
	SPITransmit( 0xff );

	SDSendCMD( CMD8 );		// send CMD8 command

	SDRead_r7( result );	// collect R7-Reponse from CMD8

	// Disable Chip Select (CS)
	SPITransmit( 0xff );
	disableSlaveSelect();
	SPITransmit( 0xff );
}


void SDSendCMD58( uint8_t *result )
{
	printf( "Start sending read_ocr (CMD58) \n" );

	// Enable Chip Select (CS)
	SPITransmit( 0xff );
	enableSlaveSelect();
	SPITransmit( 0xff );

	SDSendCMD( CMD58 );		// send CMD58 command

	SDRead_r3( result );	// collect R3-Reponse from CMD58

	// Disable Chip Select (CS)
	SPITransmit( 0xff );
	disableSlaveSelect();
	SPITransmit( 0xff );	
}


uint8_t SDReadSingleBlock( uint8_t *buffer )
{
	uint8_t r1_result, read;
	uint16_t terminate_counter;

	// Enable chip select
	SPITransmit( 0xff );
	enableSlaveSelect();
	SPITransmit( 0xff );

	// CMD17 --> (READ_SINGLE_BLOCK) initiates a block read and after completing the transfer, the card returns to the Transfer State.
	SDSendCMD( CMD17 );			// send cmd 17 to read single block from sd-card
	r1_result = SDRead_r1();	// collect R1 Response from CMD17 command

	// if response received from card
	if( r1_result != 0xff )
	{
		// wait for a response token (timeout = 100ms)
		terminate_counter = 0;
		while( ++terminate_counter != 1563 )
		{
			SPITransmit( 0xff );	// if you want to read only one byte, e.g. because you are waiting for a response from the card, you simply send a 0xff. The card does the same when it only receives data.
			if( (read = SPIReceive()) != 0xff ) break;
		}
		
		// if response token is 0xfe
		if( read == 0xfe )		// if data is in internal buffer Data Token 0xfe is send from sd-card
		{
			// read 512 byte block
			for( uint16_t i = 0; i < 512; i++ ) 
			{
				SPITransmit( 0xff );
				buffer[i] = SPIReceive();		// read 512 Bytes data from sd card and save in buffer[] 
				// Generally the card always pulls the MISO line high when it has nothing to say, i.e. you always receive 0xff. 
			}

			// // read last two CRC bytes after 512 Byte data, 16-bit CRC
			SPITransmit( 0xff );
			SPITransmit( 0xff );
		}
	}

	// Disable chip select
	SPITransmit( 0xff );
	disableSlaveSelect();
	SPITransmit( 0xff );

	return r1_result;
}


uint8_t SDWriteSingleBlock( uint8_t* data )
{
	uint8_t r1_result, read, terminate_counter = 0;

	// Enable chip select
	SPITransmit( 0xff );
	enableSlaveSelect();
	SPITransmit( 0xff );

	SDSendCMD( CMD24 );			// CMD24
	r1_result = SDRead_r1();	// R1 Response

	// if no error
	if( r1_result == 0x00 )
	{
		SPITransmit( 0xfe );	// // Start token, so that the sd-card can recognize the beginning of the block.

		// write buffer to card
		for( uint16_t i = 0; i < 512; i++ ) 
			SPITransmit( data[i] );	// write data to specified block from CMD

		// wait for a response (timeout = 250ms)
		terminate_counter = 0;
		while( ++terminate_counter != 1563 )
		{
			SPITransmit( 0xff );
			read = SPIReceive();	// answer that write was successful or not
			if( read != 0xff ) break;
		}
		
		// if data accepted
		if( (read & 0x1f) == 0x05 )		// when byte is transmitted the sd-card send an Data Response Byte. First three bit should be ignored. 0x05 means 'data accepted'.
		{
			// wait for write to finish (timeout = 250ms)
			terminate_counter = 0;
			do 
			{
				SPITransmit( 0xff );
				if( ++terminate_counter == 1563 ) break;
			} while ( SPIReceive() == 0x00 );		// if response is 0x00 than the write is completed
		}
	}

	// disable chip select
	SPITransmit( 0xff );
	disableSlaveSelect();
	SPITransmit( 0xff );

	return r1_result;
}

uint8_t isWriteProtected( void )
{
	return PINB & (1 << WP);
}

uint8_t isNoCardDetected( void )
{
	return PINB & (1 << CD);
}


void setDelay_ms( uint16_t delay )
{
	timer = 0;
	while( timer <= delay );
}

/**
 *	8bit-Timer0 ISR	
 */
ISR( TIMER0_COMP_vect )
{
	timer++;
}