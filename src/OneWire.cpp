#include "OneWire.h"
#include "IOPort.h"

#include <util/delay.h>

OneWire::OneWire(IOPort_t &port, const int pin)
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

void OneWire::WriteBit(uint8_t bit)
{
    #warning OneWire::WriteBit has no automated tests
    /*
        This is one of those "Pulsing the line" methods
        I haven't figured out how to TDD.
        It's been verified with a scope.
        Don't muck with it unless you're ready to get one out.
    */

    const int MIN_WRITE_SLOT = 60;

    //pull line low so the device knows we're about to send.
    port.DataOut &= ~datalineMask;

    if (0 == bit)
    {
        //just leave the line low to send a 0
        _delay_us(MIN_WRITE_SLOT);

        // done sending
        port.DataOut |= datalineMask;
        _delay_us(1);
    }
    else
    {
        // min wait for device to prepare to receive
        _delay_us(1);

        // pull line high to send a 1
        port.DataOut |= datalineMask;
        _delay_us(MIN_WRITE_SLOT);
    }
}