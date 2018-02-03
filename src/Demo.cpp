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
     * 
     * If we had an interrupt, but the line isn't high, assume an error state.
    */
    tempSensor._state = impl.readPin() ? Max31820State::READY_TO_READ : Max31820State::RESET;
}

IOPort_t* shiftRegisterPort = (IOPort_t*)&PINB;
ShiftRegister shiftRegister(*shiftRegisterPort);
SevenSegment sevenSeg(shiftRegister);

void display(uint16_t temp) 
{
    /* displays each digit on a single 7seg in turn*/
    const int8_t scalingFactor = 4;
    const int8_t asciiNumberOffset = 48;

    uint16_t rounded = (1 << (scalingFactor - 1)) + temp; //add 0.5
    int8_t decimal = (rounded >> scalingFactor);          //Truncate fractional (div by 2^4)

    int8_t tens = decimal / 10; 
    sevenSeg.write(tens + asciiNumberOffset);
    sevenSeg.display();
    _delay_ms(1000);

    int8_t ones = decimal % 10;
    sevenSeg.write(ones + asciiNumberOffset);
    sevenSeg.display();
    _delay_ms(1000);

    sevenSeg.write('C');
    sevenSeg.display();
    _delay_ms(1000);
}

int main(void) 
{   
    sei(); // enable global interrupt

    for(;;)
    {
        switch(tempSensor.state())
        {
            case Max31820State::RESET:
                tempSensor.requestTemperature();
                break;
            case Max31820State::WAITING:
                //_delay_ms(10);
                break;
            case Max31820State::READY_TO_READ:
                uint16_t temperature = tempSensor.readTemperature();
                display(temperature);
                _delay_ms(500); //TODO: replace with low power mode & timer/counter
                break;
        }
    }
    return 0;
}