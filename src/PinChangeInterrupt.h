#ifndef _PCI_INT_H_
#define _PCI_INT_H_

#include <stdint.h>

struct PinChangeInterrupt
{
    volatile uint8_t MaskRegister;
    volatile uint8_t ControlRegister;
    int ControlRegisterEnableIndex; 
    int Pin;
};

#endif