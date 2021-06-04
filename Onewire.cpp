#include "Onewire.h"
#include <cstdint>

Onewire::Onewire(PinName oneBus){
    pinName = oneBus;
}
void Onewire::writeBit(int bit) {
    bit = bit & 0x01;
    if (bit) {
        // Write '1' bit
        pinDown();
        NRFX_DELAY_US(A);   //6
        pinUp();
        NRFX_DELAY_US(B);   //64   
    } else {
        // Write '0' bit
        pinDown();
        NRFX_DELAY_US(C);   //60        
        pinUp();
        NRFX_DELAY_US(D);   //10
    }
}

int Onewire::readBit() {
    char result;

    pinDown();
    NRFX_DELAY_US(3);   
    pinUp();
    NRFX_DELAY_US(12);
    result = BusRead() & 0x01;
    NRFX_DELAY_US(55);
    return result;

}

uint8_t Onewire::BusRead() {
    return nrf_gpio_pin_read(pinName);
}
void Onewire::pinDown() {
    nrf_gpio_cfg_output(pinName);
    nrf_gpio_pin_clear(pinName);
}

void Onewire::pinUp() {
    nrf_gpio_cfg_input(pinName, NRF_GPIO_PIN_NOPULL);
}
int Onewire::reset(){

        uint8_t result;
        pinDown();
        NRFX_DELAY_US(480);
        pinUp();
        NRFX_DELAY_US(70);
        result = BusRead();
        NRFX_DELAY_US(410);
        return !result;
}
int Onewire::readByte() {
    int result = 0;

    for (int loop = 0; loop < 8; loop++) {
        // shift the result to get it ready for the next bit
        result >>= 1;

        // if result is one, then set MS bit
        if (readBit()) result |= 0x80;
    }
    return result;
}
void Onewire::writeByte(char data) {
    // Loop to write each bit in the byte, LS-bit first
    for (int loop = 0; loop < 8; loop++) {
        writeBit(data & 0x01);

        // shift the data byte for the next bit
        data >>= 1;
    }
}
unsigned char Onewire::CRC(unsigned char* addr, unsigned char len) {
    unsigned char i, j;
    unsigned char crc = 0;

    for (i = 0; i < len; i++) {
        unsigned char inbyte = addr[i];
        for (j = 0; j < 8; j++) {
            unsigned char mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }

    return crc;
}

uint8_t Onewire::search(uint8_t* newAddr)
{
    uint8_t         id_bit_number;
    uint8_t         last_zero, rom_byte_number, search_result;
    uint8_t         id_bit, cmp_id_bit;
 
    unsigned char   rom_byte_mask, search_direction;
 
    // initialize for search
 
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = 0;
 
    // if the last call was not the last one
    if (!LastDeviceFlag) {
        // 1-Wire reset
        volatile bool bbuf = !reset();
        if (bbuf) {
            // reset the search
            LastDiscrepancy = 0;
            LastDeviceFlag = false;
            LastFamilyDiscrepancy = 0;
            return false;
        }

        writeByte(0xF0);

        do
        {
            //NRFX_DELAY_US(10) ;
            ThisThread::sleep_for(1ms);
            // read a bit and its complement
            //id_bit = read_bit();
            //cmp_id_bit = read_bit();
            id_bit = readBit();
            cmp_id_bit = readBit();

            // check for no devices on 1-wire
            if ((id_bit == 1) && (cmp_id_bit == 1))
                break;
            else {
                // all devices coupled have 0 or 1
                if (id_bit != cmp_id_bit)
                    search_direction = id_bit;  // bit write value for search
                else {
                    // if this discrepancy if before the Last Discrepancy
                    // on a previous next then pick the same as last time
                    if (id_bit_number < LastDiscrepancy)
                        search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
                    else
                        // if equal to last pick 1, if not then pick 0
                        search_direction = (id_bit_number == LastDiscrepancy);
 
                    // if 0 was picked then record its position in LastZero
                    if (search_direction == 0) {
                        last_zero = id_bit_number;
 
                        // check for Last discrepancy in family
                        if (last_zero < 9)
                            LastFamilyDiscrepancy = last_zero;
                    }
                }
 
                // set or clear the bit in the ROM byte rom_byte_number
                // with mask rom_byte_mask
                if (search_direction == 1)
                    ROM_NO[rom_byte_number] |= rom_byte_mask;
                else
                    ROM_NO[rom_byte_number] &= ~rom_byte_mask;
 
                // serial number search direction write bit
                writeBit(search_direction);
                // increment the byte counter id_bit_number
                // and shift the mask rom_byte_mask
                id_bit_number++;
                rom_byte_mask <<= 1;
 
                // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
                if (rom_byte_mask == 0) {
                    rom_byte_number++;
                    rom_byte_mask = 1;
                }
            }
        } while (rom_byte_number < 8);
 
        // loop until through all ROM bytes 0-7
        // if the search was successful then
        if (!(id_bit_number < 65)) {
            // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
            LastDiscrepancy = last_zero;
 
            // check for last device
            if (LastDiscrepancy == 0)
                LastDeviceFlag = true;
 
            search_result = true;
        }
    }
 
    // if no device found then reset counters so next 'search' will be like a first
    if (!search_result || !ROM_NO[0]) {
        LastDiscrepancy = 0;
        LastDeviceFlag = false;
        LastFamilyDiscrepancy = 0;
        search_result = false;
    }
 
    for (int i = 0; i < 8; i++)
        newAddr[i] = ROM_NO[i];
    return search_result;
}

void Onewire::select(const uint8_t rom[8])
{
    uint8_t i;
    writeByte(0x55);
    for (i = 0; i < 8; i++)
        writeByte(rom[i]);
}

void Onewire::resetSearch(){
    LastDiscrepancy = 0;
    LastDeviceFlag = 0;
    LastFamilyDiscrepancy = 0;
}