#ifndef Onewire_h
#define Onewire_h

#include "mbed.h"

class Onewire{

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

    unsigned char ROM_NO[8];
    uint8_t LastDiscrepancy;
    uint8_t LastFamilyDiscrepancy;
    uint8_t LastDeviceFlag;

private:
  PinName pinName;
};
#endif