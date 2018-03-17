#include "SevenSegment.h"

#include <stdint.h>
#include "ShiftRegister.h"

/*
+-------------+      +-------------+
|             |      |DP           |
|      A      |      |[]    D      |
|   +-----+   |      |   +-----+   |
|  F|     |B  |      |  C|     |E  |
|   +  G  +   |      |   +  G  +   |
|    +---+    |      |    +---+    |
|   +     +   |      |   +     +   |
|  E|     |C  |      |  B|     |F  |
|   +-----+   |      |   +-----+   |
|      D    []|      |      A      |
|           DP|      |             |
+-------------+      +-------------+
     NORMAL              INVERTED


| Char | DP | G | F | E | D | C | B | A | Hex |
|------|----|---|---|---|---|---|---|---|-----|
| 0    | 0  | 0 | 1 | 1 | 1 | 1 | 1 | 1 | x3F |
| 1    | 0  | 0 | 0 | 0 | 0 | 1 | 1 | 0 | x06 |
| 2    | 0  | 1 | 0 | 1 | 1 | 0 | 1 | 1 | x5B |
| 3    | 0  | 1 | 0 | 0 | 1 | 1 | 1 | 1 | x4F |
| 4    | 0  | 1 | 1 | 0 | 0 | 1 | 1 | 0 | x66 |
| 5    | 0  | 1 | 1 | 0 | 1 | 1 | 0 | 1 | x6D |
| 6    | 0  | 1 | 1 | 1 | 1 | 1 | 0 | 1 | x7D |
| 7    | 0  | 0 | 0 | 0 | 0 | 1 | 1 | 1 | x07 |
| 8    | 0  | 1 | 1 | 1 | 1 | 1 | 1 | 1 | x7F |
| 9    | 0  | 1 | 1 | 0 | 1 | 1 | 1 | 1 | x6F |
| -    | 0  | 1 | 0 | 0 | 0 | 0 | 0 | 0 | x40 |
| C    | 0  | 0 | 1 | 1 | 1 | 0 | 0 | 1 | x39 |
| F    | 0  | 1 | 1 | 1 | 0 | 0 | 0 | 1 | x71 |
| °C   | 1  | 0 | 0 | 0 | 1 | 1 | 1 | 1 | x8F |
| °F   | 1  | 1 | 0 | 0 | 1 | 1 | 1 | 0 | xCE |

- °C/°F leverage an inverted display.
- This encoding relies on the shift register shifting in MSB first order.

*/

SevenSegment::SevenSegment(ShiftRegister& shiftRegister) : shiftRegister(shiftRegister)
{ 

}

void SevenSegment::write(uint8_t character)
{
    // We're using common cathode, so writing low activates the led.
    // Values are calculated using common annode though, so we NOT everything.
    // Common annode could easily be supported via a ctor flag.
    uint8_t data;
    switch(character)
    {
        case '0': 
            data = 0x3F;
            break;
        case '1': 
            data = 0x06;
            break;
        case '2':
            data = 0x5B;
            break;
        case '3':
            data = 0x4F;
            break;
        case '4':
            data = 0x66;
            break;
        case '5':
            data = 0x6D;
            break;
        case '6':
            data = 0x7D;
            break;
        case '7':
            data = 0x07;
            break;
        case '8':
            data = 0x7F;
            break;
        case '9':
            data = 0x6F;
            break;
        case 'C':
            data = 0x8F;
            break;
        case 'F':
            data = 0xCE;
            break;
        case '-':
            data = 0x40;
            break;
        default:
            data = 0x00; // turn the segment off if we get an unsupported char
    }

    shiftRegister.writeByte(~data);
}

void SevenSegment::display()
{
    shiftRegister.latch();
}