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

const uint8_t clockPinMask = (1 << clockPin);
const uint8_t dataPinMask = (1 << dataPin);
const uint8_t outputEnablePinMask = (1 << outputEnablePin);
const uint8_t latchPinMask = (1 << latchPin);

ShiftRegister::ShiftRegister(IOPort_t& port) : port(port)
{
    port.Direction |= registerMask; //set 0-4 to output

    uint8_t lowPins = outputEnablePinMask | clockPinMask | latchPinMask;
    port.Data = (port.Data & ~registerMask) | (registerMask & ~lowPins);
}

void ShiftRegister::writeByte(uint8_t data)
{

}

void ShiftRegister::writeBit(uint8_t value)
{
    if (0 == value)
    {
        port.Data &= ~dataPinMask;
    }
    else
    {
        port.Data |= dataPinMask;
    }

    // clock data into shift register
    port.Data |= clockPinMask;
    // pull clock low again
    port.Data &= ~clockPinMask;
}