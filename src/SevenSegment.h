#ifndef _SEVEN_SEG_H_
#define _SEVEN_SEG_H_

#include <stdint.h>
#include "ShiftRegister.h"

class SevenSegment
{
private:
    ShiftRegister& shiftRegister;
public:
    SevenSegment(ShiftRegister& shiftRegister);
    virtual void write(uint8_t character);
    virtual void display();
};

#endif