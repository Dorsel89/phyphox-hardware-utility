#include "bmp384.h"
#include <cstdint>

I2C* BMP384::myi2c;
BMP384::BMP384(I2C *myI2C_Pointer){
    myi2c = myI2C_Pointer;
}
uint8_t BMP384::changeSettings(uint8_t oversampling, uint8_t filter, uint8_t rate){
    uint16_t settings_sel;
    bmp.settings.press_en = BMP3_ENABLE;
    bmp.settings.temp_en = BMP3_ENABLE;
    //bmp.settings.odr_filter.press_os = oversampling; //BMP3_OVERSAMPLING_8X;
    bmp.settings.odr_filter.press_os = oversampling;
    bmp.settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
    bmp.settings.int_settings.drdy_en =BMP3_ENABLE;
    bmp.settings.odr_filter.iir_filter = filter;
    //bmp.settings.odr_filter.iir_filter = filter;//BMP3_IIR_FILTER_COEFF_3;
    bmp.settings.odr_filter.odr =rate;//BMP3_ODR_25_HZ;    
    //bmp.settings.odr_filter.odr = rate;//BMP3_ODR_25_HZ;//BMP3_ODR_25_HZ;    
    settings_sel = BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS | BMP3_SEL_ODR | BMP3_SEL_IIR_FILTER;
    rslt = bmp3_set_sensor_settings(settings_sel, &bmp);           
     if (rslt == BMP3_SENSOR_OK){
        bmp.settings.op_mode = BMP3_MODE_NORMAL; 
        rslt = bmp3_set_op_mode(&bmp);
        if (rslt == BMP3_SENSOR_OK)
        {
            //bmp.delay_us(40000, bmp.intf_ptr);

            /* Sensor component selection */
            sensor_comp = BMP3_PRESS | BMP3_TEMP;

            /* Temperature and Pressure data are read and stored in the bmp3_data instance */
           
        }
    }
    return 0;
}
uint8_t BMP384::init(uint8_t oversampling, uint8_t filter, uint8_t rate){

    bmp.delay_us = delay_us;
    bmp.intf_ptr = myi2c;
    bmp.intf = BMP3_I2C_INTF;
    bmp.intf_rslt = rslt;
    bmp.write = i2c_reg_write;
    bmp.read = i2c_reg_read;
    bmp3_data myData;    
    sensor_comp = BMP3_PRESS | BMP3_TEMP;
    rslt = bmp3_init(&bmp);
    if(rslt!=0){
        return 99;
    }
    
    uint16_t settings_sel;
    bmp.settings.press_en = BMP3_ENABLE;
    bmp.settings.temp_en = BMP3_ENABLE;
    bmp.settings.odr_filter.press_os = oversampling; //BMP3_OVERSAMPLING_8X;
    bmp.settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
    bmp.settings.int_settings.drdy_en =BMP3_ENABLE;
    bmp.settings.odr_filter.iir_filter = filter;//BMP3_IIR_FILTER_COEFF_3;
    bmp.settings.int_settings.level = BMP3_INT_PIN_ACTIVE_HIGH;
    bmp.settings.odr_filter.odr = rate;//BMP3_ODR_25_HZ;//BMP3_ODR_25_HZ;    
    settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS | BMP3_SEL_ODR |BMP3_SEL_DRDY_EN| BMP3_SEL_LEVEL |BMP3_SEL_IIR_FILTER;
    rslt = bmp3_set_sensor_settings(settings_sel, &bmp);           
     if (rslt == BMP3_SENSOR_OK){
        bmp.settings.op_mode = BMP3_MODE_NORMAL;
        rslt = bmp3_set_op_mode(&bmp);
        if (rslt == BMP3_SENSOR_OK)
        {
            bmp.delay_us(40000, bmp.intf_ptr);

            /* Sensor component selection */
            sensor_comp = BMP3_PRESS | BMP3_TEMP;

            /* Temperature and Pressure data are read and stored in the bmp3_data instance */
           
        }
    }
    return rslt;
}
void BMP384::getData(){
    rslt = bmp3_get_sensor_data(sensor_comp, &myData, &bmp);
    pressure = myData.pressure;
    temperature = myData.temperature;
}
uint8_t BMP384::disable(){
    bmp.settings.op_mode = BMP3_MODE_SLEEP;
    rslt = bmp3_set_op_mode(&bmp);
    return rslt;
}
uint8_t BMP384::enable(){
    bmp.settings.op_mode = BMP3_MODE_NORMAL;
    rslt = bmp3_set_op_mode(&bmp);
    return rslt;
}
void BMP384::delay_us(uint32_t period_us, void *intf_ptr){
    NRFX_DELAY_US(period_us);
}

int8_t BMP384::i2c_reg_read(uint8_t reg_addr, uint8_t *read_data, uint32_t length, void *intf_ptr){
    int8_t error;
    uint8_t databuffer[1] = {reg_addr};
    error = myi2c->write(0xEC, (const char *)&databuffer, 1, true);
    if(error){
        return error;
    }
    error = myi2c->read(0xEC|0x01, (char *)read_data, length, false);
    return error;
}

int8_t BMP384::i2c_reg_write(uint8_t reg_addr, const uint8_t *read_data, uint32_t length, void *intf_ptr){
    int8_t error;
    const uint8_t myLength  = length+1;
    uint8_t databuffer[myLength];
    databuffer[0]=reg_addr;
    for(int i =0 ;i<length;i++){
        databuffer[i+1]=read_data[i];
    }
    error = myi2c->write(0xEC,  (const char *)&databuffer[0], length+1);
    return error;
}
