#ifndef MBED_I2Chelper_H
#define MBED_I2Chelper_H

#include "mbed.h"
#include <cstdint>
#include <stdint.h>

class I2Chelper
{
    
	public:
    
	I2Chelper(I2C* i2c);
	~I2Chelper();   
    uint8_t readByte(uint8_t address, uint8_t subAddress);
    uint8_t readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
    void writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
    void writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t count, uint8_t *dest);

	private:
    I2C* i2c;
};

#endif
