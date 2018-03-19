#include "FakeOneWireImpl.h"

FakeOneWireImpl::FakeOneWireImpl(IOPort_t &port, const int pin, const long data)
    : OneWireImpl(port, pin), data(data)
{ }

uint8_t FakeOneWireImpl::readPin()
{
    //return the current bit, whatever it is and prep for the next call
    return (data >> index++) & 1;
}