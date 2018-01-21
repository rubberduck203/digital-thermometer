#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/OneWireImpl.h"
#include "../src/IOPort.h"

TEST_GROUP(OneWireImplSpec)
{
    void teardown()
    {
        mock().clear();
    }
};

TEST(OneWireImplSpec, ReadBit_Zero)
{
    mock().disable();

    const int pin = 1;
    IOPort_t port;
    port.DataIn = 0x01;

    OneWireImpl oneWire(port, pin);

    BYTES_EQUAL(0x00, oneWire.readBit());
}

TEST(OneWireImplSpec, ReadBit_One)
{
    mock().disable();

    const int pin = 1;
    IOPort_t port;
    port.DataIn = 0x02;

    OneWireImpl oneWire(port, pin);

    BYTES_EQUAL(0x01, oneWire.readBit());
}

TEST(OneWireImplSpec, ReadBit_ReadSlotTiming)
{
    // Internal detail of issueReadSlot. 
    // I dont' like having this one here,
    // but CppUTest's mocks require an expectation 
    // on every call.
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 1);

    // Wait 10us to sample per Fig. 11 & 12 of the datasheet
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 10);

    // Wait the remainder of the 60us slot before doing it again.
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 50);

    const int pin = 1;
    IOPort_t port;
    port.DataIn = 0x02;

    OneWireImpl oneWire(port, pin);
    oneWire.readBit();

    mock().checkExpectations();
}