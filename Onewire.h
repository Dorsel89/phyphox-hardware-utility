#ifndef Onewire_h
#define Onewire_h

#include "mbed.h"

class Onewire{

//normal

#define A 6
#define B 64
#define C 60
#define D 10
#define E 9 
#define F 55
#define G 0
#define H 480
#define I 70
#define J 410

//speed
/*
#define A 1
#define B 7.5
#define C 7.5
#define D 2.5
#define E 1
#define F 7
#define G 2.5
#define H 70
#define I 8.5
#define J 40
*/
public:
  Onewire(PinName oneBus);
  void writeBit(int bit);
    int readBit();
    int readByte();
    void writeByte(char data);
    unsigned char CRC(unsigned char* addr, unsigned char len);
    int reset();
    uint8_t BusRead();
    void pinUp();
    void pinDown();
    uint8_t search(uint8_t* newAddr);
    void select(const uint8_t rom[8]);
    void resetSearch();
    void targetSetup(unsigned char family_code);

    unsigned char ROM_NO[8];
    uint8_t LastDiscrepancy;
    uint8_t LastFamilyDiscrepancy;
    uint8_t LastDeviceFlag;

private:
  PinName pinName;
};
#endif