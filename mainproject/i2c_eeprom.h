#ifndef __I2C_EEPROM_H__

#define __I2C_EEPROM_H__


#include "types.h"


void i2c_eeprom_write(u8,u16,u8);

u8   i2c_eeprom_read(u8,u16);

void i2c_eeprom_page_write(u8 slaveAddr,u8 wBuffStartAddr,u8 *p,u8 nBytes);

void i2c_eeprom_seq_read(u8 slaveAddr,u8 rBuffStartAddr,u8 *p,u8 nBytes);


#endif
