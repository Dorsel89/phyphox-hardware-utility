#include "I2Chelper.h"
#include "mbed.h"
#include <cstring>

I2Chelper::I2Chelper(I2C* i2c_bus)                                                                                                             // Class constructor
{
  i2c = i2c_bus;
}
I2Chelper::~I2Chelper(){}

uint8_t I2Chelper::readByte(uint8_t I2Caddress, uint8_t subAddress)
{
    
    uint8_t data[1] = {0};
    //i2c->write(I2Caddress, (const char *)&subAddress, 1, true);
        //ThisThread::sleep_for(1ms);
    //i2c->read(I2Caddress | 0x01, (char *)data, 1, false);
        //ThisThread::sleep_for(1ms);
        readBytes(I2Caddress, subAddress, 1, &data[0]);
    return data[0];
}

void I2Chelper::writeByte(uint8_t I2C_ADDR, uint8_t regAddr, uint8_t data)
{
    uint8_t databuffer[2] = {regAddr,data};

    i2c->write(I2C_ADDR, (const char *)&databuffer[0],2);
    ThisThread::sleep_for(1ms);
    return;
}

uint8_t I2Chelper::readBytes(uint8_t I2Caddress, uint8_t subAddress, uint8_t count, uint8_t * dest)
{   i2c->lock();
    uint8_t dataBuffer[count];
    uint8_t error;
    error = i2c->write(I2Caddress, (const char *)&subAddress, 1, false);
    wait_us(10);
    if(error){
        return error;
    }
    error = i2c->read(I2Caddress | 0x01, (char *)dest, count, false);
    wait_us(10);
    i2c->unlock();

    //ThisThread::sleep_for(1ms);
    return error;
}

