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
};


TEST_GROUP(SevenSegDriverTests)
{
    void teardown()
    {
        mock().clear();
    }
};

TEST(SevenSegDriverTests, Zero)
{
    ShiftRegisterMock shiftRegister;
    mock().expectOneCall("writeByte")
            .onObject(&shiftRegister)
            .withParameter("data", (unsigned char)~0x3F);

    SevenSegment sevenSeg(shiftRegister);
    sevenSeg.display('0');

    mock().checkExpectations();
}

// Byte patterns are calculated for common annode, but we're using common cathode, 
// so we expect all bit patterns to be NOTted

// Driven by 3 shift registers.
// 3 bytes of data is shifted out

// First shift register is the 10s digit (last byte shifted in)
// Second shift register is the 1s digit (second byte shifted in)
// Third (last) shift register is the inverted display for °C/°F
