#include "MS5607.h"
#include "mbed.h"
#include <cstdint>


MS5607::MS5607(I2C* i2c)
{
    this->i2c = i2c;
}

void MS5607::init(){
    int ret = 0;
	uint8_t idx = 0;
    if(ms5607_reset() != 0) {
    	ret = -1;
        //error
    }else {
        
        for(int i =0;i<8;i++){
            I2C_ReadPROM(i);
            ThisThread::sleep_for(1ms);
        }
        
    }

}

void MS5607::readData(){
    uint8_t idx = 0;
    int ret = 0;
    ms5607_start_pressure();
    ThisThread::sleep_for(10ms);
    ms5607_read_adc_raw(&RawPressure);
    
    ms5607_start_temperature();
    ThisThread::sleep_for(10ms);
    ms5607_read_adc_raw(&RawTemp);

    ms5607_calculate();

}

void MS5607::ms5607_calculate()
{

    dT=RawTemp-CalibData[5]*pow(2,8);
    TEMP=2000+(dT*CalibData[6])/pow(2,23);

	OFF=CalibData[2]*pow(2,17)+dT*CalibData[4]/pow(2,6);
	SENS=CalibData[1]*pow(2,16)+dT*CalibData[3]/pow(2,7);
	
	P_INT=(RawPressure*SENS/pow(2,21)-OFF)/pow(2,15);
    P = 0.01 * P_INT;
    T = 0.01 *TEMP;
}

int MS5607::ms5607_reset(void)
{
	uint8_t cmd = 0x1E;
	return i2c->write(I2C_ADDR, (const char *)&cmd, 1);
}

int MS5607::ms5607_start_pressure(void)
{
	int8_t cmd = 0x48;
	return i2c->write(I2C_ADDR, (const char *)&cmd, 1);
}

int MS5607::ms5607_start_temperature(void)
{
	uint8_t cmd = 0x58;
	return i2c->write(I2C_ADDR, (const char *)&cmd, 1);
}

int MS5607::ms5607_read_adc_raw(uint32_t *pbuf)
{
	int ret = 0;
	uint8_t addr = 0x00;
	uint8_t rxbuf[3];
	if((ret = i2c->write(I2C_ADDR, (const char *)&addr, 1, true)) != 0) return ret;
	if((ret = i2c->read(I2C_ADDR | 0x01, (char *)rxbuf, 3, false)) != 0) return ret;
	pbuf[0] = (rxbuf[0] << 16) | (rxbuf[1] << 8) | rxbuf[2];
	return ret;
}

int MS5607::I2C_ReadPROM(int item)
{
    
	int ret = 0;
    uint8_t rxbuf[2] = {0, 0};
    uint8_t addr = 0xA0 + item*2;
    ret = i2c->write(I2C_ADDR, (const char *)&addr, 1, true);
    ThisThread::sleep_for(1ms);
    ret = i2c->read(I2C_ADDR | 0x01, (char *)rxbuf, 2, false);
    ThisThread::sleep_for(1ms);
    CalibData[item] = (rxbuf[0] << 8) | rxbuf[1];
	return ret;

}
