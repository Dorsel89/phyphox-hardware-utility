#ifndef MAX31850_h
#define MAX31850_h

#include "mbed.h"
#include "Onewire.h"

#define MAX31850_CONVERT_T    0x44   //  0x44
#define MAX31850_READ_POWER_SUPPLY    0xB4
#define MAX31850_READ           0xBE
 
#define MAX31850_ADC_TEMP_TC_UNIT   0.25
#define MAX31850_ADC_TEMP_CJ_UNIT   0.0625
#define MAX31850_K               41.276//41.276µV/C
 
#define MAX31850_ERR_FAULT -10;
#define MAX31850_ERR_SHORT2VDD -12;
#define MAX31850_ERR_SHORT2GND -13;
#define MAX31850_ERR_OPEN_CIRCUIT -14;


const double ITS90_TCK_subzero[] ={
0.0,
2.5173462E-2,
-1.1662878E-6,
-1.0833638E-9,
-8.9773540E-13,
-3.7342377E-16,
-8.6632643E-20,
-1.0450598E-23,
-5.1920577E-28,
0.0
};
const double ITS90_TCK_sub500[] ={
0.0,
2.508355E-02,
7.860106E-08,
-2.503131E-10,
8.315270E-14,
-1.228034E-17,
9.804036E-22,
-4.413030E-26,
1.057734E-30,
-1.052755E-35
};
const double ITS90_TCK_sub1372[] ={
-1.318058E+02,
4.830222E-02,
-1.646031E-06,
5.464731E-11,
-9.650715E-16,
8.802193E-21,
-3.110810E-26,
0.0,
0.0,
0.0
};

class MAX31850{
	public:
		MAX31850(Onewire* myOnewire);
		Onewire* myOnewire;
	
        void init();
		void searchForSensors();
		void startTempConversion();
		
        float getTemp();

        uint8_t MAX31850_ROM[8]={0};
        uint8_t dataBuffer[9]={0};
};

#endif