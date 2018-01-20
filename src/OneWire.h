#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#include "IOPort.h"

class OneWire
{
private:
    IOPort_t& _port;
    const int _pin;

public:
    OneWire(IOPort_t& port, const int pin);
    void ReleaseTx(void);
};

#endif