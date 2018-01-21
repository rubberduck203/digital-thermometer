#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#include "IOPort.h"

class OneWire
{
  private:
    IOPort_t &port;
    const int pin;
    const uint8_t datalineMask;

    /**
     * Writes a bit on the bus.
     * The caller is responsible for obtaining/releasing TX.
     */
    void writeBit(uint8_t bit);
    void issueReadSlot(void);

  public:
    OneWire(IOPort_t &port, const int pin);
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

    /**
     * Issues a read slot and samples the line.
     * Returns a uint8_t where only the MSB is populated.
     */
    uint8_t readBit();

    uint8_t read();
};

#endif