#include "ShiftRegister.h"
#include "SevenSegment.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "OneWireImpl.h"
#include "OneWire.h"
#include "Max31820.h"
#include "PinChangeInterrupt.h"

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

/* 
PIND7 is also PCINT23
An interrupt on PCINT23 triggers on PCI2 (pin change interrupt)
The PCMSK2 register controls which pins contribute to the pin change interrupts.

Pin Change Interrupt Control Register (PCICR)

Bit 2 – PCIE2: Pin Change Interrupt Enable 2
When the PCIE2 bit is set and the I-bit in the Status Register (SREG) is set, pin change interrupt 2 is
enabled. *Any change* on any enabled PCINT[23:16] pin will cause an interrupt. The corresponding
interrupt of Pin Change Interrupt Request is executed from the PCI2 Interrupt Vector. PCINT[23:16] pins
are enabled individually by the PCMSK2 Register.

| 7-3 | 2     | 1     | 0     |
| N/A | PCIE2 | PCIE1 | PCIE0 |

Pin Change Mask Register 2 PCMSK2
| 7       | 6       | 5       | 4       | 3       | 2       | 1       | 0       |
| PCINT23 | PCINT22 | PCINT21 | PCINT20 | PCINT19 | PCINT18 | PCINT17 | PCINT16 |
*/

volatile bool readyToReceive = false;
IOPort_t* oneWirePort = (IOPort_t*)&PIND;
OneWireImpl impl(*oneWirePort, PIND7);
OneWire oneWire(impl);

PinChangeInterrupt_t pci(PCICR, PCIE2, PCMSK2, PCINT23);

Max31820 tempSensor(oneWire, pci);

ISR(PCINT2_vect)
{
    /* 
     * We need to read the line state in order to determine whether
     * it's rising or falling because any change will trigger for a PCI 
    */
    readyToReceive = impl.readPin();
}

int main(void) 
{   
    const uint8_t CMD_SKIP_ROM = 0xCC;
    const uint8_t CMD_CONVERTT = 0x44;
    const uint8_t CMD_READ_SCRATCHPAD = 0xBE;

    sei();                      // enable global interrupt

    bool reset = true;
    for(;;)
    {
        if (reset) {
            tempSensor.requestTemperature();
            //if (Max31820State::WAITING == tempSensor.state())
            //    reset = false;
            
            reset = false;
        }

        if (readyToReceive)
        {
            PCMSK2 &= ~(1 << PCINT23); //disable interrupt before sending
            oneWire.reset();
            bool deviceFound = oneWire.devicePresent();

            oneWire.write(CMD_SKIP_ROM);
            oneWire.write(CMD_READ_SCRATCHPAD);

            uint8_t buffer[9];
            for(int i = 8; i >= 0; i--)
            {
                buffer[i] = oneWire.read();
            }

            readyToReceive = false;
            reset = true;
            _delay_ms(500);
        }
    }
    return 0;
}