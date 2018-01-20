#include "OneWire.h"
#include "IOPort.h"

OneWire::OneWire(IOPort_t& port, const int pin) 
    : port(port), datalineMask(1 << pin)
{
    port.Direction |= datalineMask;
}

void OneWire::ReleaseTx(void)
{
    port.Direction &= ~datalineMask;
    port.Data &= ~datalineMask;
}

void OneWire::PrepareTx(void)
{
    port.Direction |= datalineMask;
}