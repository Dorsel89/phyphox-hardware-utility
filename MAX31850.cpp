#include "MAX31850.h"

MAX31850::MAX31850(Onewire* myOnewirePointer){
    myOnewire = myOnewirePointer;
}



void MAX31850::init(){
    myOnewire->resetSearch();
    myOnewire->reset();
    myOnewire->search(MAX31850_ROM);
    ThisThread::sleep_for(100ms);
}

void MAX31850::startTempConversion(){
    myOnewire->reset();
    myOnewire->select(MAX31850_ROM);
    myOnewire->writeByte(0x44);
}

float MAX31850::getTemp(){

myOnewire->reset();
myOnewire->select(MAX31850_ROM);
myOnewire->writeByte(0xBE);
    // reading scratchpad registers
for (uint8_t i = 0; i < 9; i++){
    dataBuffer[i] = myOnewire->readByte();
} 

//---------------------------------------------------------------
float t_TC_float;
uint16_t measTC;
uint8_t *ptr_measTC;
ptr_measTC=(uint8_t*) &measTC;
*ptr_measTC = dataBuffer[0]  ;  // LSB
*++ptr_measTC = dataBuffer[1]; // MSB
if ( measTC & 0x8000 )  {

    measTC ^= 0xffff;  // convert to positive => (twos complement)++
    measTC++;
    t_TC_float= -1.0 * MAX31850_ADC_TEMP_TC_UNIT * (float)(measTC>>2);
} else {
    t_TC_float=  MAX31850_ADC_TEMP_TC_UNIT * (float)(measTC>>2);
}
volatile float t_TC_v_float = t_TC_float;
//---------------------------------------------------------------
uint8_t *ptr_meas_CJ;
uint16_t meas;
    float t_CJ_float;

ptr_meas_CJ=(uint8_t*) &meas;
*ptr_meas_CJ = dataBuffer[2]  ;  // LSB
*++ptr_meas_CJ = dataBuffer[3]; // MSB
if ( meas & 0x8000 )  {

    meas ^= 0xffff;  // convert to positive => (twos complement)++
    meas++;
    t_CJ_float= -1.0 * MAX31850_ADC_TEMP_CJ_UNIT * (float)(meas>>4);
} else {
    t_CJ_float=  MAX31850_ADC_TEMP_CJ_UNIT * (float)(meas>>4);
}
volatile float t_CJ_v_float = t_CJ_float;

//---------------------------------------------------------------
    
    
double vtc;
int err;
vtc = MAX31850_K * t_TC_float;
float realTemp = 0;
if(t_TC_float < 0.0) {
    for(int i=0;i<10;++i)
    {
    
    realTemp+=ITS90_TCK_subzero[i]*pow(vtc,i);
    }
} else if(t_TC_float < 500.0) {
    for(int i=0;i<10;++i)
    {
    
    realTemp+=ITS90_TCK_sub500[i]*pow(vtc,i);
    }
} else {
    for(int i=0;i<10;++i)
    {
    
    realTemp+=ITS90_TCK_sub1372[i]*pow(vtc,i);
    }
}
return realTemp;
}