#include "OneWire.h"
#include "IOPort.h"

#include <util/delay.h>

OneWire::OneWire(IOPort_t& port, const int pin) 
    : port(port), pin(pin), datalineMask(1 << pin)
{
    PrepareTx();
}

void OneWire::ReleaseTx(void)
{
    // Set pin to input
    port.Direction &= ~datalineMask;
    //disable pullups
    port.DataOut &= ~datalineMask;
}

void OneWire::PrepareTx(void)
{
    //set port to output
    port.Direction |= datalineMask;
}

void OneWire::Reset(void)
{
    // pull pin low for a min. of 480us
    PrepareTx();
    port.DataOut &= ~datalineMask;
    _delay_us(500);
}

bool OneWire::DevicePresent(void)
{
    ReleaseTx();
    _delay_us(60);
    bool deviceFound = !((port.DataIn >> pin) & 1);
    //wait for presence slot to close before returning
    _delay_us(180);
    return deviceFound;
}