#include "OneWire.h"
#include "IOPort.h"

OneWire::OneWire(IOPort_t& port, const int pin) : _port(port), _datalineMask(1 << pin)
{
    _port.Direction |= _datalineMask;
}

void OneWire::ReleaseTx(void)
{
    _port.Direction &= ~_datalineMask;
    _port.Data &= ~_datalineMask;
}