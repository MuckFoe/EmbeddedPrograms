#ifndef SD_H_
#define SD_H_
#include "SPI.h"

// Pins for SD-Card Holder
#define	EN1		DDB0		// Reset of SD-Card (low = running mode / high = reset)
#define EN2		DDB1		// Power-Up SD-Card Holder (low = off / high = on)
#define CD		DDB2		// Card detection (high = no card / low = card detected)
#define WP		DDB3		// Write Protection (high = SD protected / low = SD not protected)
#define LED		DDA0		// Status LED

				

// Command Pins
#define SD_R7_CHECK_PATTERN		0xaa

// Response token state
#define SD_INIT_COMPLETED		0x00
#define SD_IDLE_STATE			0x01
#define SD_ERASE_RESET			0x02

// Status message
#define SD_ERROR	1
#define SD_RUNNING	0
#define WRITE_PROTECTION_FAIL	0x02
#define CARD_DETECTION_FAIL		0x03




/*
 *	Init SD-Card.
 *	@return uint8_t error state while init.
 */
uint8_t initSD( void );

/**
 *	Init SD-Card Holder Pins and start reset of SD-Card Holder.
 *	Output Status LED.
 *	See Data sheet --> C_CONTROL_PRO_SD_CARD_INTERFACE.pdf
 */
void initSDHolder( void );

/**
 *	Initialize 8bit Timer0 in Clear timer on Compare (CTC).
 */
void initTimer0( void );

/**
 *	Status of write protection.
 *
 *	@return uint8_t
 *			1 = SD-Card is write protected
 *			0 = otherwise
 */
uint8_t isWriteProtected( void );

/**
 *	Status of card detection.
 *
 *	@return uint8_t
 *			1 = No SD-Card detected
 *			0 = otherwise
 */
uint8_t isNoCardDetected( void );

/**
 *	Chapter 6.4. Power Scheme
 *	In case of SPI mode CS (Chip Select) shall be held to high during 74 clock cycles -> 1/(F_CPU/Prescaler)*1000 = 1 clock cycle
 *	We need to provide at least 1 ms delay and 74 clock cycles before sending commands to SD Card.
 */
void SDPowerUp( void );

/**
 *	EN1= Reset SD-Card (low= running mode / high = reset)
 *	EN2= Power SD-Card Holder (low= off / high= on)
 *	Timing signal min. 50ms
 */

void resetSD( void );

/**
 *	Transmit 6 byte of CMD token
 *	Byte 1:		start bit + transmission bit + command index (6bit)
 *	Byte 2-5:	argument (32bit)
 *	Byte 3:		CRC (7bit) + end bit
 *
 *	@param cmd	command token which should be transfered.
 */
void SDSendCMD( uint8_t *cmd );

/**
 *	This response token is sent by the card after every command with the exception 
 *	of SEND_STATUS commands. Response length is 1 Byte. MSB is always set to zero.
 *	All other bits indicates an error signaled by 1.
 *	[SD Spec Chapter 7.3.2.1 Format R1]
 *	
 *	@return uint8_t R1 Response token.
*/
uint8_t SDRead_r1( void );

/**
 *	This response token is sent by the card when a READ_OCR command is received.
 *	Same length as R7-Result. Therefore R7-Result method is called.
 *	[SD Spec Chapter 7.3.2.1 Format R1]
 *
 *	@param result	Byte array that holds 5 Byte of R3-Response
 */


void SDRead_r3( uint8_t *result );

/**
 *	This response token is sent by the card when a SEND_IF_COND command (CMD8) is received. The
 *	response length is 5 bytes. The structure of the first (MSB) byte is identical to response type R1. The
 *	other four bytes contain the card operating voltage information and echo back of check pattern in
 *	argument.
 *	[SD Spec Chapter 7.3.2.6 Format R7]
 *
 *	@param result	Byte array that holds 5 Byte of R7-Response
 */
void SDRead_r7( uint8_t *result );

/**
 *	To send commands which receive an R1 response.
 *
 *	@param cmd	command which should be transfered.
 *	@return	uint8_t	R1-Response as result.
 */
uint8_t SD_CMD_with_r1_Response( uint8_t *cmd );

/**
 *	Send Interface Condition Command -> Card has to be in idle state -> checks voltage
 *	[SD Spec Chapter 4.3.13 Send Interface Condition Command (CMD8)]
 *
 *	@param result	Byte array that holds 5 Byte of R7-Response.
 */
void SDSendCMD8( uint8_t *result );


/**
 *	Read from OCR Register (operation conditions register)
 *	Shows supported voltage of SD-Card and if SD-Card has finished his power-up routine and which card type it is (SDHC/SDXC)
 *	[SD Spec Chapter 7.2.1 Mode Selection and Initialization]
 *
 *	@param result	Byte array that holds 5 Byte of R3-Response.
 */
void SDSendCMD58( uint8_t *result );

/**
 *	The SPI mode supports single block read and Multiple Block read operations (CMD17 or CMD18 in the
 *	SD Memory Card protocol). Upon reception of a valid read command the card will respond with a
 *	response token followed by a data token (Refer to Figure 7-3). In case of Standard Capacity Card, the
 *	size in the data token is determined by the block length set by SET_BLOCKLEN (CMD16). In case of
 *	SDHC and SDXC Cards, block length is fixed to 512 Bytes regardless of the block length set by CMD16.
 *	[SD Spec Chapter 7.2.3 Data Read]
 *
 *	@param buffer		buffer in which the bytes read from the SD card are to be stored.
 *	@return uint8_t		R1-Response of the CMD17.
 */
uint8_t SDReadSingleBlock( uint8_t *buffer );

/**
 *	The SPI mode supports single block and multiple block write commands. Upon reception of a valid write
 *	command (CMD24 or CMD25 in the SD Memory Card protocol), the card will respond with a response
 *	token and will wait for a data block to be sent from the host. CRC suffix, block length and start address
 *	restrictions are (with the exception of the CSD parameter WRITE_BL_PARTIAL controlling the partial
 *	block write option and WRITE_BL_LEN) identical to the read operation.
 *
 *	@param data			data to be stored on the SD-Card.
 *	@return uint8_t		R1-Response of the CMD24.
 */
uint8_t SDWriteSingleBlock( uint8_t *data );

/**
 *	Own delay method to user 8bit timer with output compare match.
 *	
 *	@param delay	delay in ms.
 */
void setDelay_ms( uint16_t delay );


#endif