#include "SevenSegmentDisplay.h"
#include "SevenSegment.h"

/*
We have 3 7-segment displays on the board.
Writing a character shifts it into the 10s position, 
so we must always write all 3 characters in before latching the display.

| 10^1 | 10^0 | °C/°F |

----

Borrowing the fixed point number format of the data reading from the Max31280.
We're going to use it for both °C and °F though.

| BIT | 15 | 14 | 13 | 12 | 11 | 10  |  9  |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
| --- | -- | -- | -- | -- | -- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|     |  S |  S |  S |  S |  S | 2^6 | 2^5 | 2^4 | 2^3 | 2^2 | 2^1 | 2^0 | 2^-1| 2^-2| 2^-3| 2^-4| 

| TEMPERATURE (°C) | DIGITAL OUTPUT (BINARY) | DIGITAL OUTPUT (HEX) |
| ---------------- | ----------------------- | -------------------- |
| +125             | 0000 0111 1101 0000     | 07D0h                |
| +85*             | 0000 0101 0101 0000     | 0550h                |
| +25.0625         | 0000 0001 1001 0001     | 0191h                |
| +10.125          | 0000 0000 1010 0010     | 00A2h                |
| +0.5             | 0000 0000 0000 1000     | 0008h                |
| 0                | 0000 0000 0000 0000     | 0000h                |
| -0.5             | 1111 1111 1111 1000     | FFF8h                |
| -10.125          | 1111 1111 0101 1110     | FF5Eh                |
| -25.0625         | 1111 1110 0110 1111     | FE6Fh                |
| -55              | 1111 1100 1001 0000     | FC90h                |

https://datasheets.maximintegrated.com/en/ds/MAX31820.pdf 
Table 1
*/

SevenSegmentDisplay::SevenSegmentDisplay(SevenSegment& driver) : driver(driver)
{
}

void SevenSegmentDisplay::write(uint16_t temp, Scale scale)
{
    const int8_t asciiNumberOffset = 48;

    uint8_t decimal = (temp >> 4);
    uint8_t ones = (decimal % 10);
    uint8_t tens = (decimal - ones) / 10;

    driver.write('C');
    driver.write(ones + asciiNumberOffset); // 10^0 
    driver.write(tens + asciiNumberOffset); // 10^1
    driver.display();
}