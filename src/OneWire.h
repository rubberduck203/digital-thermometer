#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#include "IOPort.h"

class OneWire
{
private:
    IOPort_t& port;
    const int pin;
    const uint8_t datalineMask;

public:
    OneWire(IOPort_t& port, const int pin);
    void ReleaseTx(void);
    void PrepareTx(void);
    void Reset(void);
    bool DevicePresent(void);
};

#endif