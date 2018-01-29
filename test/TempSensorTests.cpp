#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../src/Max31820.h"
#include "../src/OneWire.h"
#include "../src/OneWireImpl.h"
#include "../src/IOPort.h"

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

};

TEST(TempSensor, requestTemperature_resets)
{
    IOPort_t port;
    OneWireImpl impl(port, 0);
    MockOneWire oneWire(impl);

    mock().expectOneCall("reset")
        .onObject(&oneWire);

    mock().ignoreOtherCalls();

    Max31820 tempSensor(oneWire);
    tempSensor.requestTemperature();
}

TEST(TempSensor, requestTemperature_whenDeviceFound_SendTemperatureConversionCommand)
{
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

    mock().ignoreOtherCalls();

    Max31820 tempSensor(oneWire);
    tempSensor.requestTemperature();
}

IGNORE_TEST(TempSensor, requestTemperature_whenDeviceNOTFound)
{

}