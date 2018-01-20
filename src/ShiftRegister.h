#ifndef _SHIFT_REG_H_
#define _SHIFT_REG_H_

#include <stdint.h>
#include "IOPort.h"

class ShiftRegister
{
private:
    IOPort_t& port;
    void pulse(uint8_t bitmask);
public:
    ShiftRegister(IOPort_t& port);
    virtual void writeByte(uint8_t data);
    void writeBit(uint8_t value);
    virtual void latch();
};

#endif