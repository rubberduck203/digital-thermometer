#include "ShiftRegister.h"

// Five Pins setup for output (pins B0-B4; arduino D8-D12)
//      Data  - B4
//      OE    - B3
//      Latch - B2
//      CLK   - B1
//      CLR   - B0  

const uint8_t registerMask = 0b00011111;
const uint8_t dataPin = 4;
const uint8_t outputEnablePin = 3;
const uint8_t latchPin = 2;
const uint8_t clockPin = 1;
const uint8_t clearPin = 0;

ShiftRegister::ShiftRegister(IOPort_t& port)
{
    port.Direction |= registerMask; //set 0-4 to output

    uint8_t lowPins = (1 << outputEnablePin) | (1 << clockPin) | (1 << latchPin);
    port.Data = (port.Data & ~registerMask) | (registerMask & ~lowPins);
}

void ShiftRegister::writeByte(uint8_t data)
{

}