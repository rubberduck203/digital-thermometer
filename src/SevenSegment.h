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
    void write(uint8_t character);
    void display();
};

#endif