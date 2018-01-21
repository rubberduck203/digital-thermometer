#include "OneWireImpl.h"
#include "IOPort.h"

#include <util/delay.h>

OneWireImpl::OneWireImpl(IOPort_t &port, const int pin)
    : port(port), pin(pin), datalineMask(1 << pin)
{
    obtainTx();
}

void OneWireImpl::driveLow()
{
    port.DataOut &= ~datalineMask;
}

void OneWireImpl::driveHigh()
{
    port.DataOut |= datalineMask;
}

uint8_t OneWireImpl::readPin()
{
    return (port.DataIn >> pin) & 1;
}

void OneWireImpl::obtainTx(void)
{
    //set port to output
    port.Direction |= datalineMask;
}

void OneWireImpl::releaseTx(void)
{
    // Set pin to input
    port.Direction &= ~datalineMask;
    //disable pullups
    port.DataOut &= ~datalineMask;
}

void OneWireImpl::issueReadSlot(void)
{
    obtainTx();
    driveLow();
    _delay_us(1);
    releaseTx();
}

uint8_t OneWireImpl::readBit(void)
{
    issueReadSlot();
    _delay_us(10);
    uint8_t result = readPin();
    _delay_us(50);
    return result;
}

void OneWireImpl::writeBit(uint8_t bit)
{
    const int MIN_WRITE_SLOT = 60;

    //alert the slave we're about to send.
    driveLow();

    if (0 == bit)
    {
        //just leave the line low to send a 0
        _delay_us(MIN_WRITE_SLOT);

        // done sending
        driveHigh();
        _delay_us(1);
    }
    else
    {
        // min wait for device to prepare to receive
        _delay_us(1);

        // pull line high to send a 1
        driveHigh();
        _delay_us(MIN_WRITE_SLOT);
    }
}