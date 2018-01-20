#include "OneWire.h"
#include "IOPort.h"

OneWire::OneWire(IOPort_t& port, const int pin) : _port(port), _pin(pin)
{
    _port.Direction |= (1 << _pin);
}

void OneWire::ReleaseTx(void)
{
    _port.Direction &= ~(1 << _pin);
    _port.Data &= ~(1 << _pin);
}