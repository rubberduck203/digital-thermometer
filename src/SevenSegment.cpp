#include "SevenSegment.h"

#include <stdint.h>
#include "ShiftRegister.h"

SevenSegment::SevenSegment(ShiftRegister& shiftRegister) : shiftRegister(shiftRegister)
{
    
}

void SevenSegment::display(uint8_t character)
{
    if ('0' == character)
    {
        shiftRegister.writeByte(~0x3F);
    }
}