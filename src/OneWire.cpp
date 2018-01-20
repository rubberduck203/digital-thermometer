#include "OneWire.h"
#include "IOPort.h"

#include <util/delay.h>

OneWire::OneWire(IOPort_t& port, const int pin) 
    : port(port), datalineMask(1 << pin)
{
    PrepareTx();
}

void OneWire::ReleaseTx(void)
{
    // Set pin to input
    port.Direction &= ~datalineMask;
    //disable pullups
    port.Data &= ~datalineMask;
}

void OneWire::PrepareTx(void)
{
    //set pin to output
    port.Direction |= datalineMask;
}

void OneWire::Reset(void)
{
    // pull pin low for a min. of 480us
    PrepareTx();
    port.Data &= ~datalineMask;
    _delay_us(500);
}