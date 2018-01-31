#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/Max31820.h"
#include "../src/OneWire.h"
#include "../src/OneWireImpl.h"
#include "../src/IOPort.h"
#include "../src/PinChangeInterrupt.h"

namespace TempSensorSpec {

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

IOPort_t port;
OneWireImpl impl(port, 0);
MockOneWire oneWire(impl);

TEST_GROUP(TempSensor)
{
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(TempSensor, requestTemperature_resets)
{
    uint8_t ctlReg = 0;
    uint8_t pinMaskReg = 0;
    PinChangeInterrupt_t pci(ctlReg, 0, pinMaskReg, 0);

    mock().expectOneCall("reset")
        .onObject(&oneWire);

    mock().ignoreOtherCalls();

    Max31820 tempSensor(oneWire, pci);
    tempSensor.requestTemperature();
}

TEST(TempSensor, onCreate_InResetMode)
{
    uint8_t ctlReg = 0;
    uint8_t pinMaskReg = 0;
    PinChangeInterrupt_t pci(ctlReg, 0, pinMaskReg, 0);

    mock().disable();

    Max31820 tempSensor(oneWire, pci);
    LONGS_EQUAL(Max31820State::RESET, tempSensor.state())
}

TEST(TempSensor, onCreate_InterruptMaskRegisterIsCleared)
{
    uint8_t ctlReg = 0;
    uint8_t pinMaskReg = 0;
    PinChangeInterrupt_t pci(ctlReg, 0, pinMaskReg, 0);
    pci.MaskRegister = 0xFF;

    Max31820 tempSensor(oneWire, pci);

    BYTES_EQUAL(0x00, pci.MaskRegister);
}

TEST(TempSensor, onCreate_InterruptPortIsEnabled)
{
    uint8_t ctlReg = 0x01; //some other thing has cr1 enabled
    const int ctlRegEnableIdx = 1;
    uint8_t pinMaskReg = 0;
    PinChangeInterrupt_t pci(ctlReg, ctlRegEnableIdx, pinMaskReg, 0);

    Max31820 tempSensor(oneWire, pci);

    BYTES_EQUAL(0x03, pci.ControlRegister); // we've enabled our pci w/out messing with anyone else
}

TEST(TempSensor, requestTemperature_whenDeviceFound)
{
    uint8_t ctlReg = 0;
    uint8_t pinMaskReg = 0;
    const int interruptPin = 3;
    PinChangeInterrupt_t pci(ctlReg, 0, pinMaskReg, interruptPin);

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

    Max31820 tempSensor(oneWire, pci);
    tempSensor.requestTemperature();

    LONGS_EQUAL(Max31820State::WAITING, tempSensor.state());
    BYTES_EQUAL(0x08, pci.MaskRegister);
}

TEST(TempSensor, requestTemperature_whenDeviceNOTFound)
{
    uint8_t ctlReg = 0;
    uint8_t pinMaskReg = 0;
    const int interruptPin = 3;
    PinChangeInterrupt_t pci(ctlReg, 0, pinMaskReg, interruptPin);

    mock().expectOneCall("devicePresent")
        .onObject(&oneWire)
        .andReturnValue(false);

    mock().expectNoCall("write");
    mock().expectNoCall("issueReadSlot");

    mock().ignoreOtherCalls();

    Max31820 tempSensor(oneWire, pci);
    tempSensor.requestTemperature();

    //maybe ERROR would be better? DEVICE_NOT_FOUND?
    LONGS_EQUAL(Max31820State::RESET, tempSensor.state());
    BYTES_EQUAL(0x00, pci.MaskRegister); //all interrupt pins are still disabled
}

IGNORE_TEST(TempSensor, readTemp_deviceFound)
{
    uint8_t ctlReg = 0;
    uint8_t pinMaskReg = 0;
    const int interruptPin = 3;
    PinChangeInterrupt_t pci(ctlReg, 0, pinMaskReg, interruptPin);

    // interrupt pin disabled
    // reset 
    // device found == true
    // skip rom
    // read scratchpad
    // verify 9 byte buffer read
    // state = reset
    FAIL("NEEDS IMPLEMENTED");
}

} //namespace