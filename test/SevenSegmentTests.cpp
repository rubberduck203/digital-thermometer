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

TEST(SevenSegDriverTests, Two)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x5B);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('2');
}

TEST(SevenSegDriverTests, Three)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x4F);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('3');
}

TEST(SevenSegDriverTests, Four)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x66);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('4');
}

TEST(SevenSegDriverTests, Five)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x6D);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('5');
}

TEST(SevenSegDriverTests, Six)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x7D);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('6');
}

TEST(SevenSegDriverTests, Seven)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x07);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('7');
}

TEST(SevenSegDriverTests, Eight)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x7F);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('8');
}

TEST(SevenSegDriverTests, Nine)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, ~0x6F);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('9');
}

TEST(SevenSegDriverTests, DegreesCelcius)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, (unsigned char)~0x8F);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('C');
}

TEST(SevenSegDriverTests, DegreesFarenheit)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, (unsigned char)~0xCE);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('F');
}

TEST(SevenSegDriverTests, Negative)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, (unsigned char)~0x40);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('-');
}

TEST(SevenSegDriverTests, unsupportedCharacterTurnsSegmentOff)
{
    ShiftRegisterMock shiftRegister;
    expectByteWritten(shiftRegister, (unsigned char)0xFF);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.write('Z');
}

TEST(SevenSegDriverTests, displayLatchesRegisters)
{
    ShiftRegisterMock shiftRegister;
    mock().expectOneCall("latch")
            .onObject(&shiftRegister);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.display();
}


