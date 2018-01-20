#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/OneWire.h"
#include "../src/IOPort.h"


TEST_GROUP(OneWireSpec)
{

};

TEST(OneWireSpec, OnInitialization_DirectionIsSetToOutput)
{
    const int pin = 7;
    IOPort_t port;
    port.Direction = 0x00;

    OneWire oneWire(port, pin);
    BITS_EQUAL(0b10000000, port.Direction, 0xFF);
}

