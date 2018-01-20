#include "OneWire.h"
#include "IOPort.h"

OneWire::OneWire(IOPort_t& port, const int pin)
{
    port.Direction |= (1 << pin);
}