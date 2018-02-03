#ifndef _MAX31820_H_
#define _MAX31820_H_

#include "OneWire.h"
#include "PinChangeInterrupt.h"

enum class Max31820State: uint8_t
{
     RESET,
     WAITING,
     READY_TO_READ,
};

extern "C"
{
    void PCINT2_vect(void) __attribute__ ((signal));
}

class Max31820 
{
private:
    OneWire &oneWire;
    Max31820State _state;
    PinChangeInterrupt_t &_pci;

public:
    Max31820(OneWire &oneWire, PinChangeInterrupt_t &pci);
    
    /*** 
     * Initializes a temperature conversion. 
     * Puts state() into WAITING.
     * When conversion is complete, state() returns READY
     */
    void requestTemperature();

    /***
     * Must only call after state() == READY 
     * @returns 16bit fixed point number a format of SSSSS0000000.0000
     */
    uint16_t readTemperature();

    Max31820State state();

friend void PCINT2_vect(void);
};

#endif