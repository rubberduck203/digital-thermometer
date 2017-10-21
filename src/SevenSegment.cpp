#include "SevenSegment.h"

#include <stdint.h>
#include "ShiftRegister.h"

SevenSegment::SevenSegment(ShiftRegister& shiftRegister) : shiftRegister(shiftRegister)
{ 

}

void SevenSegment::write(uint8_t character)
{
    // We're using common cathode, so writing low activates the led.
    // Values are calculated using common annode though, so we NOT everything.
    // Common annode could easily be supported via a ctor flag.
    uint8_t data = 0xFF;
    switch(character)
    {
        case '0': 
            data = 0x3F;
            break;
        case '1': 
            data = 0x06;
            break;
    }

    shiftRegister.writeByte(~data);
}