#ifndef BMP384_H
#define BMP384_H
#define BMP3_SENSOR_OK                                      UINT8_C(0)
#include "bmp3.h"
#include "mbed.h"


class BMP384{
    public:
		BMP384(I2C *myI2C_Pointer);
        uint8_t init();
        
        float pressure;
        float temperature;
        void getData();
        uint8_t disable();
        static I2C* myi2c;
    private:
        
        void test(void *a);
        static void delay_us(unsigned int period_us, void *intf_ptr);
        static int8_t i2c_reg_write(uint8_t reg_addr, const uint8_t *read_data, uint32_t length, void *intf_ptr);
        static int8_t i2c_reg_read(uint8_t reg_addr, uint8_t *read_data, uint32_t length, void *intf_ptr);
        int8_t rslt;
        uint8_t sensor_comp;
        struct bmp3_dev bmp;
        bmp3_data myData;
};


#endif
