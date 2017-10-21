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
    void display(uint8_t character);
};

#endif