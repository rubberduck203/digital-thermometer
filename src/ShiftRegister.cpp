#include "ShiftRegister.h"

// Five Pins setup for output (pins B0-B4; arduino D8-D12)
//      Data  - B4 - D12
//      OE    - B3 - D11
//      Latch - B2 - D10
//      CLK   - B1 - D09
//      CLR   - B0 - D08

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
    * Right shift the data bit we're interested in into the right most bit we're interested in.
    * ANDing with 1 gives us a HIGH/LOW value to write
    */ 
    for(uint8_t i = 0; i < 8; i++)
    {
        writeBit((data >> i) & 1);
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

    pulse(clockPinMask);
}

void ShiftRegister::latch()
{
    pulse(latchPinMask);
}

void ShiftRegister::pulse(uint8_t bitmask)
{
    // pull the pin high to clock the data, 
    // then back to low so we choose when to send it.
    port.Data |= bitmask;
    port.Data &= ~bitmask;

    // Hoping the tens of nanoseconds timing requirements are met here.
    // I haven't quite figured out how long it takes to do a digital write to GPIO.
    // If things are wonky, add a microsecond delay before driving clock pin high
    // and again before driving it low again. 
}