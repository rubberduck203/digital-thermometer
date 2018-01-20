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
    void WriteBit(uint8_t bit);

  public:
    OneWire(IOPort_t &port, const int pin);
    void ReleaseTx(void);
    void PrepareTx(void);
    void Reset(void);

    /**
     * After a Reset, 
     * call this to check for a device on the bus.
     * Returns true if found, otherwise false. 
     */
    bool DevicePresent(void);

    /**
     * Writes one byte to the bus LSB first.
     */
    void Write(uint8_t data);
};

#endif