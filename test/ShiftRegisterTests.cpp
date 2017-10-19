#include <stdint.h>
#include <avr/io.h>
#include "CppUTest/TestHarness.h"
#include "../src/ShiftRegister.h"

/* 
    DDRxn - 1 is output, 0 is input
    PORTxn
        - When Input: 0 enables pullup resistor
        - When Output: 0 drives line LOW; 1 drives line HIGH
    PINx retrieves values of all pinson port when in input mode 
*/ 

// Five Pins setup for output (pins B0-B4; arduino D8-D12)
//      Data  - B4
//      OE    - B3
//      Latch - B2
//      CLK   - B1
//      CLR   - B0  

// Latch is pulled low before shifting bits out
// Latch is pulled high after data is shifted out

TEST_GROUP(ShiftRegisterDriverTests)
{
};

TEST(ShiftRegisterDriverTests, OnInitialization_PinsSetupAsOutput)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Direction = 0x00;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0b00011111, shiftRegPort.Direction, 0xFF);
}

TEST(ShiftRegisterDriverTests, OnInitalization_DirectionFor_PinsFiveSixAndSeven_AreNotModified)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Direction = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0b11100000, shiftRegPort.Direction, 0b11100000);
}

TEST(ShiftRegisterDriverTests, OnInitalization_DataOn_PinsFiveSixAndSeven_IsNotModified)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0x00;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0b00011111, shiftRegPort.Data, 0b11100000);
}

TEST(ShiftRegisterDriverTests, OnIntialization_OutputEnableIsDrivenLow)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0b11110111, shiftRegPort.Data, 0x08);
    // it would be far nicer to have bool shiftRegister.OutputEnabled()
}

TEST(ShiftRegisterDriverTests, OnInitialization_ClearIsDrivenHigh)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0x00;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0x1, shiftRegPort.Data, 0x01);
}

TEST(ShiftRegisterDriverTests, OnInitialization_ClockIsDrivenLow)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0x00, shiftRegPort.Data, 0x02);
}

TEST(ShiftRegisterDriverTests, OnInitialization_LatchIsDrivenLow)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0x00, shiftRegPort.Data, 0x04);
}

const int clockPinMask = 0x02;
const int latchPinMask = 0x04;
const int dataPinMask = 0x10;

TEST(ShiftRegisterDriverTests, ShiftHighOut)
{
    IOPort_t shiftRegPort;
    ShiftRegister shiftRegister(shiftRegPort);
    shiftRegPort.Data = 0x00; //don't ever set this directly like this in prod. Just setting up the test.
    BITS_EQUAL(0x00, shiftRegPort.Data, dataPinMask);
    shiftRegister.writeBit(1);
    BITS_EQUAL(0xFF, shiftRegPort.Data, dataPinMask);

    //verify we haven't messed with other pins
    BITS_EQUAL(0x00, shiftRegPort.Data, ~dataPinMask);
}

TEST(ShiftRegisterDriverTests, ShiftLowOut)
{
    IOPort_t shiftRegPort;
    ShiftRegister shiftRegister(shiftRegPort);
    shiftRegPort.Data = 0xFF; //don't ever set this directly like this in prod. Just setting up the test.
    BITS_EQUAL(0xFF, shiftRegPort.Data, dataPinMask);
    shiftRegister.writeBit(0);
    BITS_EQUAL(0x00, shiftRegPort.Data, dataPinMask);

    //verify we haven't messed with other pins (except clock pin)
    BITS_EQUAL(0xFF, shiftRegPort.Data, ~dataPinMask & ~clockPinMask);
}

TEST(ShiftRegisterDriverTests, AfterShiftBitOut_ClockIsLow)
{
    // So, I don't know how to test that the clock went high, then low
    // before the writeBit() method exits. 
    // So, I wrote the test, added the write high, which broke it, then added the write low.
    // Not exactly ideal, but the functionality was verified once and other tests broke when I did it.
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);
    shiftRegister.writeBit(1);

    BITS_EQUAL(0x00, shiftRegPort.Data, clockPinMask);
}

TEST(ShiftRegisterDriverTests, AfterLatch_LatchIsLow)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);
    shiftRegister.latch();

    BITS_EQUAL(0x00, shiftRegPort.Data, latchPinMask);
}

IGNORE_TEST(ShiftRegisterDriverTests, AfterLatch_NoOtherPinsHaveBeenModified)
{
    
}

IGNORE_TEST(ShiftRegisterDriverTests, WritesAByte)
{
    IOPort_t shiftRegPort;
    ShiftRegister shiftRegister(shiftRegPort);
    shiftRegister.writeByte(0x01);
    //BYTE_EQUALS(0X01, ?How to spy?)
    // for each bit in byte write bit.
    // I have no idea how to test this besides verifying the final state 
    //  and that we've not mucked with other pins.
}