#include "ShiftRegister.h"

ShiftRegister::ShiftRegister(IOPort_t& port)
{
    port.Direction = 0x1F; //set 0-4 to output
}

void ShiftRegister::writeByte(uint8_t data)
{

}