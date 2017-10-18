#include "ShiftRegister.h"

const uint8_t registerMask = 0b00011111;
const uint8_t outputEnablePin = 1;

ShiftRegister::ShiftRegister(IOPort_t& port)
{
    port.Direction |= registerMask; //set 0-4 to output
    port.Data = (port.Data & ~registerMask) | (registerMask & ~(1 << outputEnablePin));
}

void ShiftRegister::writeByte(uint8_t data)
{

}