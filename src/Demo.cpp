#include "ShiftRegister.h"
#include <avr/io.h>
#include <util/delay.h>

// /* Shift Register Driver Test 1 */
// int main(void) 
// {   
//     IOPort_t* shiftRegisterPort = (IOPort_t*)&DDRB;
//     ShiftRegister shiftRegister(*shiftRegisterPort);
//     for(;;)
//     {
//         for(uint8_t i = 0; i < 255; i++)
//         {
//             shiftRegister.writeByte(i);
//             shiftRegister.latch();
//             _delay_ms(1000);
//         }
//     }
//     return 0;
// }

int main(void) 
{   
    IOPort_t* shiftRegisterPort = (IOPort_t*)&DDRB;
    ShiftRegister shiftRegister(*shiftRegisterPort);

    int i = 0;
    for(;;)
    {
        const uint8_t Five  = 0x6D;
        const uint8_t Nine  = 0x6F;

        uint8_t character = i % 2 ? Five : Nine;
        shiftRegister.writeByte(~character);
        shiftRegister.latch();
        i++;
        _delay_ms(500);
    }
    return 0;
}