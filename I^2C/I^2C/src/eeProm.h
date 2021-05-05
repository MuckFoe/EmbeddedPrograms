/*
 * eeProm.h
 *
 * Created: 05/05/2021 17:50:57
 *  Author: Moe
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "twi.h"
#define EEPAGES 64
#define EEPAGESIZE 16
#define EEMAXADDR 0x0800
#define SLA_W 0b10100000
#define SLA_R 0b10100001



uint8_t EEWriteByte(uint16_t u16address, uint8_t u8data);
uint8_t EEReadByte(uint16_t u16address, uint8_t *u8data);
uint8_t EEReadPage(uint8_t page, uint8_t *u8data);
uint8_t EEWritePage(uint8_t page, uint8_t *u8data);


#endif /* EEPROM_H_ */