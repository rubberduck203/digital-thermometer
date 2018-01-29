#ifndef _PCI_INT_H_
#define _PCI_INT_H_

#include <stdint.h>

struct PinChangeInterrupt_t
{
    volatile uint8_t& ControlRegister;
    const int ControlRegisterEnableIndex; 
    volatile uint8_t& MaskRegister;
    const int Pin;

    PinChangeInterrupt_t(volatile uint8_t& ctlReg, int ctlRegEnableIdx, volatile uint8_t& pinMaskReg, int pin)
        : ControlRegister(ctlReg), ControlRegisterEnableIndex(ctlRegEnableIdx), MaskRegister(pinMaskReg), Pin(pin)
    {}
    //todo: maybe this should have some methods like "isEnabled", "enable()", "disable()", etc.
};

#endif