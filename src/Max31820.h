#ifndef _MAX31820_H_
#define _MAX31820_H_

#include "OneWire.h"
#include "PinChangeInterrupt.h"

enum class Max31820State: uint8_t
{
     RESET,
     WAITING,
};

class Max31820 
{
private:
    OneWire &oneWire;
    Max31820State _state;
    PinChangeInterrupt_t &_pci;

public:
    Max31820(OneWire &oneWire, PinChangeInterrupt_t &pci);
    void requestTemperature();
    Max31820State state();
};

#endif