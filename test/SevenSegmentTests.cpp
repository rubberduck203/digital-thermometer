#include <stdint.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/SevenSegment.h"

#include <avr/io.h>
#include "../src/ShiftRegister.h"
class ShiftRegisterMock : public ShiftRegister
{
public:
    ShiftRegisterMock() : ShiftRegister(fakePort) {}
    virtual void writeByte(uint8_t data)
    {
        mock().actualCall("writeByte")
                .onObject(this)
                .withUnsignedIntParameter("data", data);
    }

    virtual void latch()
    {
        mock().actualCall("latch")
                .onObject(this);
    }
};


TEST_GROUP(SevenSegDriverTests)
{
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

void expectByteWritten(ShiftRegister& shiftRegister, unsigned char value)
{
    mock().expectOneCall("writeByte")
        .onObject(&shiftRegister)
        .withParameter("data", (unsigned char)value);
}

TEST(SevenSegDriverTests, Zero)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x3F);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('0');
}

TEST(SevenSegDriverTests, One)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x06);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('1');
}

TEST(SevenSegDriverTests, displayLatchesRegisters)
{
    ShiftRegisterMock shiftRegister;
    mock().expectOneCall("latch")
            .onObject(&shiftRegister);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.display();
}

IGNORE_TEST(SevenSegDriverTests, unsupportedCharacter)
{

}

// Driven by 3 shift registers.
// 3 bytes of data is shifted out

// First shift register is the 10s digit (last byte shifted in)
// Second shift register is the 1s digit (second byte shifted in)
// Third (last) shift register is the inverted display for °C/°F
