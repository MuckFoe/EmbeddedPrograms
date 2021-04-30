#ifndef __SPISD_H_
#define __SPISD_H_

#include <avr/io.h>


// SD card instruction list
#define cmd0 0	 // card reset
#define cmd1 1   // command 1, init
#define cmd8 8   // command 8, SEND_IF_COND
#define cmd9 9   // command 9, read CSD data
#define cmd10 10 // command 10, read CID data
#define cmd12 12 // command 12, stop data transmission
#define cmd16 16 // command 16, setting SectorSize should return 0x00
// read cmd17
#define cmd18 18 // command 18, read Multi sector
#define cmd23 23 // command 23. Set N block s to be erased before writing multiple sections
#define cmd24 24 // command 24, write section
#define cmd25 25 // command 25, write Multi sector
#define cmd41 41 // command 41, should return 0x00
#define cmd55 55 // command 55, return 0x01
#define cmd58 58 // command 58, read OCR information
#define cmd59 59 // command 59, enable / disable CRC, return 0x00

// There are many kinds of responses in SD card. R1 is the standard response, which is most commonly used. Similar to R1 response are R1b, R2 and R3.
// R1 response in addition to send_ Other commands except status are sent later, and the highest bit is sent first, with a total of 1 byte. The highest bit is 0. The response is described as follows:
// 0x01: idle state
// 0x02: erase error
// 0x04: command error
// 0x08: CRC communication error
// 0x10: erase order error
// 0x20: address error
// 0x40: parameter error

#define MSD_RESPONSE_NO_Error 0x00 // no error
#define MSD_IN_IDLE_State 0x01 // idle state
#define MSD_ERASE_Reset 0x02 // erase error
#define MSD_ILLEGAL_Command 0x04 // command error
#define MSD_COM_CRC_Error 0x08 // CRC communication error
#define MSD_ERASE_SEQUENCE_Error 0x10 // erase order error
#define MSD_ADDRESS_Error 0x20 // address error
#define MSD_PARAMETER_Error 0x40 // parameter error
#define MSD_RESPONSE_Failure 0xff // the command failed at all and there was no response


// Pins
#define SCK PB7
#define MISO PB6
#define MOSI PB5
#define SS PB4
#define LED PA0
#define LEDArray PORTC

#endif