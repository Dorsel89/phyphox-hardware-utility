#include "MPRLS.h"
#include "mbed.h"
MPRLS::MPRLS(uint16_t PSI_min, uint16_t PSI_max,
             float OUTPUT_min, float OUTPUT_max, float K) {

  _PSI_min = PSI_min;
  _PSI_max = PSI_max;
  _OUTPUT_min = (uint32_t)((float)COUNTS_224 * (OUTPUT_min / 100.0) + 0.5);
  _OUTPUT_max = (uint32_t)((float)COUNTS_224 * (OUTPUT_max / 100.0) + 0.5);
  _K = K;
}

/**************************************************************************/
/*!
    @brief  setup and initialize communication with the hardware
    @param i2c_addr The I2C address for the sensor (default is 0x18)
    @param twoWire Optional pointer to the desired TwoWire I2C object. Defaults
   to &Wire
    @returns True on success, False if sensor not found
*/
/**************************************************************************/
void MPRLS::setI2C(uint8_t i2c_addr, I2C *twoWire) {
  _i2c_addr = i2c_addr;
  _i2c = twoWire;

  ThisThread::sleep_for(10ms);

  // Serial.print("Status: ");
  // Serial.println(stat);
}

/**************************************************************************/
/*!
    @brief Read and calculate the pressure
    @returns The measured pressure, in hPa on success, NAN on failure
*/
/**************************************************************************/
float MPRLS::readPressure(void) {
  uint32_t raw_psi = readData();
  if (raw_psi == 0xFFFFFFFF || _OUTPUT_min == _OUTPUT_max) {
    return NAN;
  }

  // All is good, calculate and convert to desired units using provided factor
  // use the 10-90 calibration curve by default or whatever provided by the user
  float psi = (raw_psi - _OUTPUT_min) * (_PSI_max - _PSI_min);
  psi /= (float)(_OUTPUT_max - _OUTPUT_min);
  psi += _PSI_min;
  // convert to desired units
  return psi * _K;
}

/**************************************************************************/
/*!
    @brief Read 24 bits of measurement data from the device
    @returns -1 on failure (check status) or 24 bits of raw ADC reading
*/
/**************************************************************************/
uint32_t MPRLS::readData(void) {
    
    char databuf[3] = {0xAA,0x00,0x00};
    _i2c->write(_i2c_addr,databuf,3,1);


    ThisThread::sleep_for(50ms);
    char receiveBuffer[4];
    _i2c->read(_i2c_addr,receiveBuffer,4);


  uint32_t ret = receiveBuffer[1] << 16 | receiveBuffer[2] << 8  | receiveBuffer[3];
  return ret;
}
