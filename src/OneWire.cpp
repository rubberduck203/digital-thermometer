#include "OneWire.h"
#include "IOPort.h"

#include <util/delay.h>

OneWire::OneWire(OneWireImpl &impl)
    : impl(impl)
{ }

void OneWire::releaseTx(void)
{
    impl.releaseTx();
}

void OneWire::obtainTx(void)
{
    impl.obtainTx();
}

void OneWire::reset(void)
{
    // pull pin low for a min. of 480us
    impl.obtainTx();
    impl.driveLow();
    _delay_us(500);
}

bool OneWire::devicePresent(void)
{
    impl.releaseTx();
    _delay_us(60);
    bool deviceFound = !impl.readPin();
    //wait for presence slot to close before returning
    _delay_us(180);
    return deviceFound;
}

void OneWire::write(uint8_t data)
{    
    impl.obtainTx();
  
    // write least significant bit first
    for(int8_t i = 0; i < 8; i++) {
        impl.writeBit(!!(data & (1 << i)));
    }

    impl.releaseTx();
}

uint8_t OneWire::read(void)
{
    #warning OneWire::read has no automated tests
    /* OneWire protocol specifies data is sent
        LSB and LSb first. */
    uint8_t buffer = 0;
    for(int i = 0; i < 8; i++)
    {
        buffer |= (impl.readBit() << i);
    }
    return buffer;
}