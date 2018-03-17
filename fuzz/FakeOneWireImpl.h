#ifndef _FAKE_ONEWIRE_IMPL_H_
#define _FAKE_ONEWIRE_IMPL_H_

#include "../src/OneWireImpl.h"
#include "../src/IOPort.h"

/**
 * Fakes the readBit() method so we can get better fuzzing.
 */ 
class FakeOneWireImpl : public OneWireImpl
{
private:
    int data;
    int index = 0;
public:
    FakeOneWireImpl(IOPort_t &port, const int pin, int data);
    virtual uint8_t readPin() override;
};

#endif //_FAKE_ONEWIRE_IMPL_H_