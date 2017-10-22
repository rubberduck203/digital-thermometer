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
    uint8_t data;
    switch(character)
    {
        case '0': 
            data = 0x3F;
            break;
        case '1': 
            data = 0x06;
            break;
        case '2':
            data = 0x5B;
            break;
        case '3':
            data = 0x4F;
            break;
        case '4':
            data = 0x66;
            break;
        case '5':
            data = 0x6D;
            break;
        case '6':
            data = 0x7D;
            break;
        case '7':
            data = 0x07;
            break;
        case '8':
            data = 0x7F;
            break;
        case '9':
            data = 0x6F;
            break;
        case 'C':
            data = 0x8F;
            break;
        case 'F':
            data = 0xCE;
            break;
        default:
            data = 0x00; // turn the segment off if we get an unsupported char
    }

    shiftRegister.writeByte(~data);
}

void SevenSegment::display()
{
    shiftRegister.latch();
}