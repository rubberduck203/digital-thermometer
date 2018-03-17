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
    virtual void driveLow();

    /**
     * Drive the bus high.
     * Must call obtainTx() first.
     */
    void driveHigh();

    /**
     * Read pin state
     */
    virtual uint8_t readPin();
    
    /**
     * Releases the bus so slaves can send data.
     * The pull up resistor will pull the line high.
     */
    virtual void releaseTx();

    /**
     * Sets the master's bus line to output.
     */
    virtual void obtainTx();

    /**
     * Issues a read slot and samples the line.
     * Returns a uint8_t where only the MSB is populated.
     */
    virtual uint8_t readBit();

    /**
     * Writes a bit on the bus.
     * The caller is responsible for obtaining/releasing TX.
     */
    virtual void writeBit(uint8_t bit);

    /** 
     * Generates a read slot.
     * Caller is NOT responsible fore obtaining/releasing TX.
     */
    void issueReadSlot();
};

#endif