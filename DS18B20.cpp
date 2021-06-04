#include "DS18B20.h"

DS18B20::DS18B20(Onewire* myOnewirePointer){
    myOnewire = myOnewirePointer;
}

void DS18B20::init(){
    myOnewire->resetSearch();
    myOnewire->reset();
    myOnewire->search(DS18B20_ROM);
    ThisThread::sleep_for(100ms);
}

void DS18B20::startTempConversion(){
    myOnewire->reset();
    myOnewire->select(DS18B20_ROM);
    myOnewire->writeByte(0x44);
}

float DS18B20::getTemp(){
    myOnewire->reset();
    myOnewire->select(DS18B20_ROM);
    myOnewire->writeByte(0xBE);
    // reading scratchpad registers
    for (uint8_t i = 0; i < 9; i++){
        dataBuffer[i] = myOnewire->readByte();
    } 
    uint16_t buffer[1] = {0};
    memcpy(&buffer[0], &dataBuffer[0], 2);
    return 0.0625 * buffer[0];
}