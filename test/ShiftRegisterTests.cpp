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

IGNORE_TEST(ShiftRegisterDriverTests, OnIntialization_OutputEnableIsDrivenLow)
{
    
}

IGNORE_TEST(ShiftRegisterDriverTests, WritesAByte)
{
    IOPort_t shiftRegPort;
    ShiftRegister shiftRegister(shiftRegPort);
    shiftRegister.writeByte(0x01);
    //BYTE_EQUALS(0X01, ?How to spy?)
}