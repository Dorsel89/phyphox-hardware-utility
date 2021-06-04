#ifndef DS18B20_h
#define DS18B20_h

#include "mbed.h"
#include "Onewire.h"

class DS18B20{
	public:
		DS18B20(Onewire* myOnewire);
		Onewire* myOnewire;
	
        void init();
		void searchForSensors();
		void startTempConversion();
		
        float getTemp();

        uint8_t DS18B20_ROM[8]={0};
        uint8_t dataBuffer[9]={0};
};

#endif