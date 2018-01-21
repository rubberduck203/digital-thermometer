#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#include "IOPort.h"
#include "OneWireImpl.h"

/**
 * Lightweight (i.e. incomplete) Maxim OneWire implementation.
 */
class OneWire
{
  private:
    OneWireImpl &impl;
  public:
    OneWire(OneWireImpl &impl);

    void releaseTx(void);
    void obtainTx(void);

    /**
     * Sends a reset pulse.
     */ 
    void reset(void);

    /**
     * After a Reset, 
     * call this to check for a device on the bus.
     * Returns true if found, otherwise false. 
     */
    bool devicePresent(void);

    /**
     * Writes one byte to the bus LSB first.
     */
    void write(uint8_t data);

    /***
     * Reads one byte.
     */
    uint8_t read();
};

#endif