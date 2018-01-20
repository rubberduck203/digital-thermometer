#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#include "IOPort.h"

class OneWire
{
public:
    OneWire(IOPort_t& port, const int pin);
};

#endif