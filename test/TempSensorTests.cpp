#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/Max31820.h"
#include "../src/OneWire.h"
#include "../src/OneWireImpl.h"
#include "../src/IOPort.h"
#include "../src/PinChangeInterrupt.h"

TEST_GROUP(TempSensor)
{
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

class MockOneWire: public OneWire
{
public:
    MockOneWire(OneWireImpl impl): OneWire(impl) { }

    virtual void reset()
    {
        mock().actualCall("reset")
            .onObject(this);
    }

    virtual bool devicePresent()
    {
        return mock().actualCall("devicePresent")
            .onObject(this)
            .returnBoolValue();
    }

    virtual void write(uint8_t data)
    {
        mock().actualCall("write")
            .onObject(this)
            .withUnsignedIntParameter("data", data);
    }

    virtual void issueReadSlot()
    {
        mock().actualCall("issueReadSlot")
            .onObject(this);
    }

};

TEST(TempSensor, requestTemperature_resets)
{
    PinChangeInterrupt pci;
    IOPort_t port;
    OneWireImpl impl(port, 0);
    MockOneWire oneWire(impl);

    mock().expectOneCall("reset")
        .onObject(&oneWire);

    mock().ignoreOtherCalls();

    Max31820 tempSensor(oneWire, pci);
    tempSensor.requestTemperature();
}

TEST(TempSensor, onCreate_InResetMode)
{
    PinChangeInterrupt pci;
    IOPort_t port;
    OneWireImpl impl(port, 0);
    MockOneWire oneWire(impl);

    mock().disable();

    Max31820 tempSensor(oneWire, pci);
    LONGS_EQUAL(Max31820State::RESET, tempSensor.state())
}

TEST(TempSensor, onCreate_InterruptMaskRegisterIsCleared)
{
    PinChangeInterrupt pci;
    pci.MaskRegister = 0xFF;

    IOPort_t port;
    OneWireImpl impl(port, 0);
    MockOneWire oneWire(impl); 

    Max31820 tempSensor(oneWire, pci);

    BYTES_EQUAL(0x00, pci.MaskRegister);
}

TEST(TempSensor, onCreate_InterruptPortIsEnabled)
{
    PinChangeInterrupt pci;
    pci.ControlRegister = 0x01; //some other thing has cr1 enabled
    pci.ControlRegisterEnableIndex = 1;

    IOPort_t port;
    OneWireImpl impl(port, 0);
    MockOneWire oneWire(impl); 

    Max31820 tempSensor(oneWire, pci);

    BYTES_EQUAL(0x03, pci.ControlRegister); // we've enabled our pci w/out messing with anyone else
}

TEST(TempSensor, requestTemperature_whenDeviceFound)
{
    PinChangeInterrupt pci;
    IOPort_t port;
    OneWireImpl impl(port, 0);
    MockOneWire oneWire(impl);

    mock().expectOneCall("devicePresent")
        .onObject(&oneWire)
        .andReturnValue(true);

    mock().expectOneCall("write")
        .onObject(&oneWire)
        .withUnsignedIntParameter("data", 0xCC); //skip rom; we only have one sensor on the bus
    
    mock().expectOneCall("write")
        .onObject(&oneWire)
        .withUnsignedIntParameter("data", 0x44); //ConvertTemp

    mock().expectOneCall("issueReadSlot")
        .onObject(&oneWire);

    mock().ignoreOtherCalls();

    pci.Pin = 3;
    Max31820 tempSensor(oneWire, pci);
    tempSensor.requestTemperature();

    LONGS_EQUAL(Max31820State::WAITING, tempSensor.state());
    BYTES_EQUAL(0x08, pci.MaskRegister);
}

IGNORE_TEST(TempSensor, requestTemperature_whenDeviceNOTFound)
{
    FAIL("HANDLE ERROR CASE")
}