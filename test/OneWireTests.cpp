#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/OneWire.h"
#include "../src/IOPort.h"


TEST_GROUP(OneWireSpec)
{
    void teardown()
    {
        mock().clear();
    }
};

void _delay_us(double __us) 
{
    mock().actualCall("_delay_us")
        .withDoubleParameter("__us", __us);
}

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

TEST(OneWireSpec, Reset_DirectionIsSetToOutput)
{
    mock().disable();
    const int pin = 0;
    IOPort_t port;
    port.Direction = 0x00;

    OneWire oneWire(port, pin);
    oneWire.ReleaseTx();
    oneWire.Reset();

    BYTES_EQUAL(0X01, port.Direction);
}

TEST(OneWireSpec, Reset_OutputIsPulledLowForAMinimumOf480us)
{
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 500);
    
    const int pin = 0;
    IOPort_t port;
    port.Data = 0xFF;

    OneWire oneWire(port, pin);
    oneWire.Reset();

    BYTES_EQUAL(0xFE, port.Data);
    mock().checkExpectations();
}