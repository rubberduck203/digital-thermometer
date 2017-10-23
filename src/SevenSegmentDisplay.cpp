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

#include "SevenSegmentDisplay.h"
#include "SevenSegment.h"

SevenSegmentDisplay::SevenSegmentDisplay(SevenSegment& driver) : driver(driver)
{
}

void SevenSegmentDisplay::write(uint16_t temp, Scale scale)
{
    // printf("\n%X", temp);
    // R = F/(2^f); Real = Fixed / (2^fractional bits)
    // float actual = (float)(int16_t)temp / 16; 
    // printf("\n%f", actual);

    const int8_t scalingFactor = 4;
    const int8_t asciiNumberOffset = 48;

    uint8_t tempScaleChar = (Celcius == scale) ? 'C' : 'F';

    uint16_t rounded = (1 << (scalingFactor - 1)) + temp; //add 0.5
    int8_t decimal = (rounded >> scalingFactor);          //Truncate fractional (div by 2^4)

    // Shift the sign bit into the Least Significant Bit
    bool negative = (temp >> 15);
    if (negative)
    {
        // -4 mod 10 = 6 ... we need the abs value here.
        int8_t positive = decimal * -1;
        int8_t ones = positive % 10; 
        int8_t tens = positive / 10; // Allow int cast to discard the remainder.

        if (0 == tens)
        {
            driver.write(tempScaleChar);
            driver.write(ones + asciiNumberOffset); 
            driver.write('-'); 
        }
        else{
            driver.write(ones + asciiNumberOffset);
            driver.write(tens + asciiNumberOffset);
            driver.write('-');
        }
    }
    else 
    {    
        int8_t ones = decimal % 10;
        int8_t tens = decimal / 10; 

        driver.write(tempScaleChar);
        driver.write(ones + asciiNumberOffset);
        // If we have over 9 tens, then we need to mod this to get the right number. Do it everytime for simplicity.
        driver.write((tens % 10) + asciiNumberOffset); 

        if (tens >= 10)
        {
            int8_t hundreds = decimal / 100; 
            driver.write(hundreds + asciiNumberOffset);
        }
    }

    driver.display();
}