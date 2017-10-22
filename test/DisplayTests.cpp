#include <stdint.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/ShiftRegister.h"
#include "../src/SevenSegment.h"
#include "../src/SevenSegmentDisplay.h"

TEST_GROUP(DisplayTests)
{
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};


IOPort_t port;
ShiftRegister shiftReg(port);

class SevenSegmentMock: public SevenSegment
{
public:
    SevenSegmentMock() : SevenSegment(shiftReg)
    {
    }

    void write(uint8_t character)
    {
        mock().actualCall("write")
                .onObject(this)
                .withParameter("character", character);
    }

    void display()
    {
        mock().actualCall("display")
                .onObject(this);
    }
};

/*

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
*/

void expectDisplayed(SevenSegment& driver)
{
    mock().expectOneCall("display")
        .onObject(&driver);
}

void expectWrite(SevenSegment& driver, char character)
{
    mock().expectOneCall("write")
        .onObject(&driver)
        .withParameter("character", (unsigned char)character);
}

void expectCelciusWritten(SevenSegment& driver)
{
    expectWrite(driver, 'C');
}

TEST(DisplayTests, displayCelciusZero)
{
    SevenSegmentMock driver;

    expectCelciusWritten(driver);
    expectWrite(driver, '0');
    expectWrite(driver, '0');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x0000, SevenSegmentDisplay::Celcius);
}

TEST(DisplayTests, displayCelciusOne)
{
    SevenSegmentMock driver;

    expectCelciusWritten(driver);
    expectWrite(driver, '1');
    expectWrite(driver, '0');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x0010, SevenSegmentDisplay::Celcius);
}

TEST(DisplayTests, displayCelciusFive)
{
    SevenSegmentMock driver;

    expectCelciusWritten(driver);
    expectWrite(driver, '5');
    expectWrite(driver, '0');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x0050, SevenSegmentDisplay::Celcius);
}

TEST(DisplayTests, displayCelciusTwoDigitNumber)
{
    SevenSegmentMock driver;

    expectCelciusWritten(driver);
    expectWrite(driver, '0');
    expectWrite(driver, '1');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x00A0, SevenSegmentDisplay::Celcius);
}

TEST(DisplayTests, displayCelciusTwoDigitNumberWithNonZeroOnesDigit)
{
    SevenSegmentMock driver;

    expectCelciusWritten(driver);
    expectWrite(driver, '2');
    expectWrite(driver, '1');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x00C0, SevenSegmentDisplay::Celcius);
}

TEST(DisplayTests, roundUpFromHalf)
{
    SevenSegmentMock driver;
    
    expectCelciusWritten(driver);
    expectWrite(driver, '1');
    expectWrite(driver, '3');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x00C8, SevenSegmentDisplay::Celcius);
}

TEST(DisplayTests, roundDownFromLessThanHalf)
{
    SevenSegmentMock driver;
    
    expectCelciusWritten(driver);
    expectWrite(driver, '1');
    expectWrite(driver, '2');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x00C7, SevenSegmentDisplay::Celcius);
}

IGNORE_TEST(DisplayTests, ThreeDigits)
{
    FAIL("BOOM!");
}

IGNORE_TEST(DisplayTests, NegativeTemps)
{
    FAIL("BOOM!");
}

TEST(DisplayTests, Farenheit)
{
    SevenSegmentMock driver;
    
    expectWrite(driver, 'F');
    expectWrite(driver, '1');
    expectWrite(driver, '2');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x00C0, SevenSegmentDisplay::Farenheit);
}

TEST(DisplayTests, NineBitResolution)
{
    SevenSegmentMock driver;
    
    expectWrite(driver, 'F');
    expectWrite(driver, '7');
    expectWrite(driver, '2');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x01B0, SevenSegmentDisplay::Farenheit);
}