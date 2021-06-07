#ifndef MBED_MS5607_H
#define MBED_MS5607_H

#include "mbed.h"
#include <cstdint>
#include <stdint.h>

class MS5607
{
    
	public:
    
	MS5607(I2C* i2c);
	
	int ms5607_reset(void);
	int ms5607_start_pressure(void);
	int ms5607_start_temperature(void);
	int ms5607_read_adc_raw(uint32_t *pbuf);
	void ms5607_calculate();
	int I2C_ReadPROM(int item);
	void readData();
    void init();
	const uint8_t I2C_ADDR = 0xEC;
    
	uint16_t CalibData[8];
    uint32_t RawTemp,RawPressure;
	int32_t dT;
    int32_t TEMP;
    int64_t OFF, SENS;
    int32_t P_INT;
    float T;
    float P;

	private:
    void compensateSecondOrder(void);
    I2C* i2c;
};

#endif
