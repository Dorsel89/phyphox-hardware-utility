#ifndef DS18B20_H
#define DS18B20_h

#include "mbed.h"
#include "Onewire_h"

class DS18B20{
	public:
		DS18B20(Onewire myOnewire);
		Onewire myOnewire;
	
		void searchForSensors();
		void startTempConversion();
		float32 getData;
}

#endif