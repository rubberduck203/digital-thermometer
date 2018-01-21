#ifndef _ONEWIRE_IMPL_H_
#define _ONEWIRE_IMPL_H_

#include "IOPort.h"

/**
 * Low level OneWire functionality.
 * NOT for public use.
 * Largely exists for testability.
 * Use OneWire instead.
 * 
 * The code in this class was unit tested when feasible, 
 * but was largely verified with a scope.
 * The idea is to isolate "untested" code in this class.
 */ 
class OneWireImpl
{
  private:
    IOPort_t &port;
    const int pin;
    const uint8_t datalineMask;

  public:
    OneWireImpl(IOPort_t &port, const int pin);
    /**
     * Drive the bus low.
     * Must call obtainTx() first.
     */
    void driveLow();

    /**
     * Drive the bus high.
     * Must call obtainTx() first.
     */
    void driveHigh();

    /**
     * Read pin state
     */
    uint8_t readPin();
    
    /**
     * Releases the bus so slaves can send data.
     * The pull up resistor will pull the line high.
     */
    void releaseTx(void);

    /**
     * Sets the master's bus line to output.
     */
    void obtainTx(void);

    /**
     * Issues a read slot and samples the line.
     * Returns a uint8_t where only the MSB is populated.
     */
    uint8_t readBit();

    /**
     * Writes a bit on the bus.
     * The caller is responsible for obtaining/releasing TX.
     */
    void writeBit(uint8_t bit);

    /** 
     * Generates a read slot.
     * Caller is NOT responsible fore obtaining/releasing TX.
     */
    void issueReadSlot(void);
};

#endif