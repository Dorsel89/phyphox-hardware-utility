#ifndef MPRLS_h
#define MPRLS_h

#include "mbed.h"
#define MPRLS_DEFAULT_ADDR (0x18)   ///< Most common I2C address
#define MPRLS_READ_TIMEOUT (20)     ///< millis
#define MPRLS_STATUS_POWERED (0x40) ///< Status SPI powered bit
#define MPRLS_STATUS_BUSY (0x20)    ///< Status busy bit
#define MPRLS_STATUS_FAILED (0x04)  ///< Status bit for integrity fail
#define MPRLS_STATUS_MATHSAT (0x01) ///< Status bit for math saturation
#define COUNTS_224 (16777216L)      ///< Constant: 2^24
#define PSI_to_HPA (68.947572932)   ///< Constant: PSI to HPA conversion factor
#define MPRLS_STATUS_MASK                                                      \
  (0b01100101) ///< Sensor status mask: only these bits are set

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with MPRLS
   sensor IC
   Ported from Arduino MPRLS Library to MBED OS (some features are not implemented yet)
*/
/**************************************************************************/
class MPRLS {
public:
  MPRLS(uint16_t PSI_min = 0, uint16_t PSI_max = 25,
                 float OUTPUT_min = 10, float OUTPUT_max = 90,
                 float K = PSI_to_HPA);

  void setI2C(uint8_t i2c_addr = MPRLS_DEFAULT_ADDR, I2C* myI2C = nullptr);

  float readPressure(void);

  uint8_t lastStatus; /*!< status byte after last operation */

private:
  uint32_t readData(void);

  uint8_t _i2c_addr;
  uint16_t _PSI_min, _PSI_max;
  uint32_t _OUTPUT_min, _OUTPUT_max;
  float _K;

  I2C *_i2c;
};
#endif