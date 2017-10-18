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

// Output Enable Pin is pulled low on initalization
// Clear pin is pulled high on initialization
// Clock is pulled low on initalization
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

    BYTES_EQUAL(0b00011111, shiftRegPort.Direction);
}

TEST(ShiftRegisterDriverTests, OnInitalization_DirectionFor_PinsFiveSixAndSeven_AreNotModified)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Direction = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);

    BYTES_EQUAL(0xFF, shiftRegPort.Direction);
}

TEST(ShiftRegisterDriverTests, OnInitalization_DataOn_PinsFiveSixAndSeven_IsNotModified)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0x00;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0b00011101, shiftRegPort.Data, 0xFF);
    BYTES_EQUAL(0b00011101, shiftRegPort.Data);
}

TEST(ShiftRegisterDriverTests, OnIntialization_OutputEnableIsDrivenLow)
{
    IOPort_t shiftRegPort;
    shiftRegPort.Data = 0xFF;
    ShiftRegister shiftRegister(shiftRegPort);

    BITS_EQUAL(0b11111101, shiftRegPort.Data, 0xFF);
    BYTES_EQUAL(0b11111101, shiftRegPort.Data);
    // it would be far nicer to have bool shiftRegister.OutputEnabled()
}

IGNORE_TEST(ShiftRegisterDriverTests, WritesAByte)
{
    IOPort_t shiftRegPort;
    ShiftRegister shiftRegister(shiftRegPort);
    shiftRegister.writeByte(0x01);
    //BYTE_EQUALS(0X01, ?How to spy?)
}