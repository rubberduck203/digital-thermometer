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
    /* 
    * The double negate turns non-zero into a one.
    * There's no real need for it so long as the implementation of writeBit() remains unchanged 
    * but we'll keep this just in case because there aren't really tests around this.
    */ 
    for(uint8_t i = 0; i < 8; i++)
    {
        uint8_t mask = 1 << i;
        writeBit(!!(data & mask));
    }
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

    // Hoping the tens of nanoseconds timing requirements are met here.
    // I haven't quite figured out how long it takes to do a digital write to GPIO.
    // If things are wonky, add a microsecond delay before driving clock pin high
    // and again before driving it low again. 
}