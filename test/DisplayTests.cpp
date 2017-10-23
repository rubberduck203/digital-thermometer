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

/*
Fixed point format of "temp" argument.

| BIT | 15 | 14 | 13 | 12 | 11 | 10  |  9  |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
| --- | -- | -- | -- | -- | -- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|Value|  S |  S |  S |  S |  S | 2^6 | 2^5 | 2^4 | 2^3 | 2^2 | 2^1 | 2^0 | 2^-1| 2^-2| 2^-3| 2^-4| 

*/

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

TEST(DisplayTests, SingleDigitNegativeTemps)
{
    SevenSegmentMock driver;
    
    expectWrite(driver, 'F');
    expectWrite(driver, '4');
    expectWrite(driver, '-');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    // -4F
    display.write(0xFFC0, SevenSegmentDisplay::Farenheit);
}

TEST(DisplayTests, DoubleDigitNegativeTemps)
{
    SevenSegmentMock driver;
    
    expectWrite(driver, '5');
    expectWrite(driver, '2');
    expectWrite(driver, '-');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    // -25.0625 == 0b1111 1110 0110 1111 == 0xFE6F
    display.write(0xFE6F, SevenSegmentDisplay::Farenheit);
}

IGNORE_TEST(DisplayTests, NeedToFixTripleDigitEdgeCase)
{
    // TODO: Segment 3 is inverted to get the ° symbol.
    // I realized late in the game that we can't simply just shift into that space.
    // The control codes for the 3rd segment are different.

    FAIL("Segment 3 is inverted to get the ° symbol.");
}

TEST(DisplayTests, ThreeDigits)
{
    SevenSegmentMock driver;
    
    expectWrite(driver, 'F');
    expectWrite(driver, '0');
    expectWrite(driver, '0');
    expectWrite(driver, '1'); // We just shift out the °F to use all 3 segments.
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x0640, SevenSegmentDisplay::Farenheit);
}

TEST(DisplayTests, ThreeDigitsWithTensDigit)
{
    SevenSegmentMock driver;
    
    expectWrite(driver, 'F');
    expectWrite(driver, '5');
    expectWrite(driver, '2');
    expectWrite(driver, '1');
    expectDisplayed(driver);

    SevenSegmentDisplay display(driver);
    display.write(0x07D0, SevenSegmentDisplay::Farenheit);
}