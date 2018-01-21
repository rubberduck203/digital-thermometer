#include "ShiftRegister.h"
#include "SevenSegment.h"
#include <avr/io.h>
#include <util/delay.h>
#include "OneWireImpl.h"
#include "OneWire.h"

// /* Shift Register Driver Demo 1 */
// int main(void) 
// {   
//     IOPort_t* shiftRegisterPort = (IOPort_t*)&PINB;
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

// /* Shift Register Demo 2 */
// int main(void) 
// {   
//     IOPort_t* shiftRegisterPort = (IOPort_t*)&PINB;
//     ShiftRegister shiftRegister(*shiftRegisterPort);

//     int i = 0;
//     for(;;)
//     {
//         const uint8_t Five  = 0x6D;
//         const uint8_t Nine  = 0x6F;

//         uint8_t character = i % 2 ? Five : Nine;
//         shiftRegister.writeByte(~character);
//         shiftRegister.latch();
//         i++;
//         _delay_ms(500);
//     }
//     return 0;
// }

// /* 7-Seg Driver Demo */
// int main(void) 
// {   
//     IOPort_t* shiftRegisterPort = (IOPort_t*)&PINB;
//     ShiftRegister shiftRegister(*shiftRegisterPort);

//     SevenSegment sevenSeg(shiftRegister);

//     const uint8_t Characters[] = {'0','1','2','3','4','5','6','7','8','9','C','F'};

//     for(;;)
//     {
//         int8_t i;
//         for(i = 0; i < 12; i++)
//         {
//           sevenSeg.write(Characters[i]);
//           sevenSeg.display();
//           _delay_ms(1000);
//         }
//     }
//     return 0;
// }

/* OneWire Demo (best viewed w/ logic analyzer) */
int main(void) 
{   
    const uint8_t CMD_SKIP_ROM = 0xCC;
    const uint8_t CMD_CONVERTT = 0x44;
    const uint8_t CMD_READ_SCRATCHPAD = 0xBE;

    IOPort_t* oneWirePort = (IOPort_t*)&PIND;
    OneWireImpl impl(*oneWirePort, PIND7);
    OneWire oneWire(impl);

    for(;;)
    {
        oneWire.reset();
        bool deviceFound = oneWire.devicePresent();

        oneWire.write(CMD_SKIP_ROM);
        oneWire.write(CMD_CONVERTT);

        //todo: create an abstraction for waiting for conversion
        impl.issueReadSlot();
        while(!impl.readPin()) {
            //todo: use an interrupt instead
            //todo: add a timeout?
        }

        oneWire.reset();
        deviceFound = oneWire.devicePresent();

        oneWire.write(CMD_SKIP_ROM);
        oneWire.write(CMD_READ_SCRATCHPAD);

        uint8_t buffer[9];
        for(int i = 8; i >= 0; i--)
        {
            buffer[i] = oneWire.read();
        }
    }
    return 0;
}