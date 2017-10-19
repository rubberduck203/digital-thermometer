#include "ShiftRegister.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void) 
{   
    //TODO: setup and loop
    IOPort_t* shiftRegisterPort = (IOPort_t*)&DDRB;
    ShiftRegister shiftRegister(*shiftRegisterPort);
    for(;;)
    {
        for(uint8_t i = 0; i < 255; i++)
        {
            shiftRegister.writeByte(i);
            shiftRegister.latch();
            _delay_ms(1000);
        }
    }
    return 0;
}