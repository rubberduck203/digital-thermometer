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

TEST(OneWireSpec, ReleaseTx_DirectionIsSetToOutput)
{
    const int pin = 7;
    IOPort_t port;
    port.Direction = 0x00;
    
    OneWire oneWire(port, pin);
    oneWire.ReleaseTx();

    BYTES_EQUAL(0x00, port.Direction);
}

TEST(OneWireSpec, ReleaseTx_PullupsAreDisabled)
{
    const int pin = 7;
    IOPort_t port;
    port.Data = 0xFF;
    
    OneWire oneWire(port, pin);
    oneWire.ReleaseTx();

    BITS_EQUAL(0b01111111, port.Data, 0xFF);
}

TEST(OneWireSpec, PrepareTx_DirectionIsSetToOutput)
{
    const int pin = 6;
    IOPort_t port;
    port.Direction = 0x00;

    OneWire oneWire(port, pin);
    oneWire.ReleaseTx();
    oneWire.PrepareTx();
    
    BITS_EQUAL(0b01000000, port.Direction, 0xFF);
}