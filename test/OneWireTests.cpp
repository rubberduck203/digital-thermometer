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

    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    BITS_EQUAL(0b10000000, port.Direction, 0xFF);
}

TEST(OneWireSpec, ReleaseTx_DirectionIsSetToOutput)
{
    const int pin = 7;
    IOPort_t port;
    port.Direction = 0x00;
    
    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    oneWire.releaseTx();

    BYTES_EQUAL(0x00, port.Direction);
}

TEST(OneWireSpec, ReleaseTx_PullupsAreDisabled)
{
    /* 
    Atmel gpio is tri-state. 
    We must disable the internal pullups 
    so that our external pullup can pull the line high.
    */
    const int pin = 7;
    IOPort_t port;
    port.DataOut = 0xFF;
    
    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    oneWire.releaseTx();

    BITS_EQUAL(0b01111111, port.DataOut, 0xFF);
}

TEST(OneWireSpec, ObtainTx_DirectionIsSetToOutput)
{
    const int pin = 6;
    IOPort_t port;
    port.Direction = 0x00;

    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    oneWire.releaseTx();
    oneWire.obtainTx();

    BITS_EQUAL(0b01000000, port.Direction, 0xFF);
}

TEST(OneWireSpec, Reset_DirectionIsSetToOutput)
{
    mock().disable();
    const int pin = 0;
    IOPort_t port;
    port.Direction = 0x00;

    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    oneWire.releaseTx();
    oneWire.reset();

    BYTES_EQUAL(0X01, port.Direction);
}

TEST(OneWireSpec, Reset_OutputIsPulledLowForAMinimumOf480us)
{
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 500);
    
    const int pin = 0;
    IOPort_t port;
    port.DataOut = 0xFF;

    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    oneWire.reset();

    BYTES_EQUAL(0xFE, port.DataOut);
    mock().checkExpectations();
}

TEST(OneWireSpec, DevicePresent_Receives)
{
    mock().disable();

    const int pin = 1;
    IOPort_t port;
    port.Direction = 0xFF;
    port.DataOut = 0xFF;

    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    oneWire.devicePresent();

    //set to input
    BYTES_EQUAL(0XFD, port.Direction);
    //pullups disabled
    BYTES_EQUAL(0XFD, port.DataOut);
}

TEST(OneWireSpec, DevicePresent_ListensFor60usThenWaitsForPresenceSlotToClose)
{
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 60);
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 180);

    IOPort_t port;
    OneWireImpl impl(port, 3);
    OneWire oneWire(impl);

    oneWire.devicePresent();
    mock().checkExpectations();
}

TEST(OneWireSpec, DevicePresent_ReturnsTrueWhenLineLow)
{
    mock().disable();

    const int pin = 0;
    IOPort_t port;
    port.DataIn = 0x00;

    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    CHECK(oneWire.devicePresent());
}

TEST(OneWireSpec, DevicePresent_ReturnsFalseWhenLineHigh)
{
    mock().disable();

    const int pin = 0;
    IOPort_t port;
    port.DataIn = 0xFF;

    OneWireImpl impl(port, pin);
    OneWire oneWire(impl);
    CHECK_FALSE(oneWire.devicePresent());
}

class ImplMock : public OneWireImpl
{
public:
    ImplMock(IOPort_t port, const int pin) 
        : OneWireImpl(port, pin) {}

    virtual void obtainTx()
    {
        mock().actualCall("obtainTx")
                .onObject(this);
    }

    virtual void releaseTx()
    {
        mock().actualCall("releaseTx")
                .onObject(this);
    }

    virtual void writeBit(uint8_t bit)
    {
        mock().actualCall("writeBit")
            .onObject(this)
            .withUnsignedIntParameter("bit", bit);
    }

    virtual uint8_t readBit()
    {
        mock().actualCall("readBit")
                .onObject(this);

        return mock().unsignedIntReturnValue();
    }

    virtual void driveLow()
    {
        mock().actualCall("driveLow")
            .onObject(this);
    }
};

TEST(OneWireSpec, write_ObtainsTx)
{
    IOPort_t port;
    const int pin = 4;
    ImplMock impl(port, pin);

    mock().expectOneCall("obtainTx")
        .onObject(&impl);
    mock().ignoreOtherCalls();

    OneWire oneWire(impl);
    oneWire.write(1);

    mock().checkExpectations();
}

TEST(OneWireSpec, write_ReleasesTx)
{
    IOPort_t port;
    const int pin = 4;
    ImplMock impl(port, pin);

    mock().expectOneCall("releaseTx")
        .onObject(&impl);
    mock().ignoreOtherCalls();

    OneWire oneWire(impl);
    oneWire.write(1);

    mock().checkExpectations();
}

void expectBitWrite(OneWireImpl &impl, uint8_t bit, int callIndex)
{
    mock().expectOneCall("writeBit")
        .onObject(&impl)
        .withUnsignedIntParameter("bit", bit)
        .withCallOrder(callIndex);
}

TEST(OneWireSpec, write_SendsLeastSignificantBitFirst)
{
    IOPort_t port;
    const int pin = 4;
    ImplMock impl(port, pin);

    expectBitWrite(impl, 1, 1);
    expectBitWrite(impl, 0, 2);
    expectBitWrite(impl, 1, 3);
    expectBitWrite(impl, 0, 4);
    expectBitWrite(impl, 1, 5);
    expectBitWrite(impl, 0, 6);
    expectBitWrite(impl, 1, 7);
    expectBitWrite(impl, 0, 8);

    mock().ignoreOtherCalls();

    OneWire oneWire(impl);
    oneWire.write(0b01010101);

    mock().checkExpectations();
}

TEST(OneWireSpec, issueReadSlot) 
{
    IOPort_t port;
    const int pin = 4;
    ImplMock impl(port, pin);

    mock().expectOneCall("obtainTx")
        .onObject(&impl);
    mock().expectOneCall("driveLow")
        .onObject(&impl);
    mock().expectOneCall("_delay_us")
        .withDoubleParameter("__us", 1);
    mock().expectOneCall("releaseTx")
        .onObject(&impl);

    OneWire oneWire(impl);
    oneWire.issueReadSlot();

    mock().checkExpectations();
}

void expectReadBit(OneWireImpl &impl, uint8_t bit, int callIndex)
{
    mock().expectOneCall("readBit")
        .onObject(&impl)
        .withCallOrder(callIndex)
        .andReturnValue(bit);
}

TEST(OneWireSpec, Read_ReadsOneByteLeastSignificantBitFirst)
{
    IOPort_t port;
    ImplMock impl(port, 4);

    expectReadBit(impl, 0, 1);
    expectReadBit(impl, 1, 2);
    expectReadBit(impl, 0, 3);
    expectReadBit(impl, 1, 4);
    expectReadBit(impl, 0, 5);
    expectReadBit(impl, 1, 6);
    expectReadBit(impl, 0, 7);
    expectReadBit(impl, 1, 8);

    mock().ignoreOtherCalls();

    OneWire oneWire(impl);
    BITS_EQUAL(0b10101010, oneWire.read(), 0xFF);
}