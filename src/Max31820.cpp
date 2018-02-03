#include "Max31820.h"

const uint8_t CMD_SKIP_ROM = 0xCC;
const uint8_t CMD_CONVERTT = 0x44;
const uint8_t CMD_READ_SCRATCHPAD = 0xBE;

Max31820::Max31820(OneWire &oneWire, PinChangeInterrupt_t &pci): oneWire(oneWire), _pci(pci)
{ 
    _state = Max31820State::RESET;
    _pci.MaskRegister = 0x00; // disable pin change interrupts for all pins on port
    _pci.ControlRegister |= (1 << _pci.ControlRegisterEnableIndex);
}

void Max31820::requestTemperature()
{
    oneWire.reset();

    if (!oneWire.devicePresent())
    {
        //no devices present, try again
        _state = Max31820State::RESET;
        return; //return false? errorcode?
    }

    oneWire.write(CMD_SKIP_ROM);
    oneWire.write(CMD_CONVERTT);

    oneWire.issueReadSlot();
    _state = Max31820State::WAITING;
    _pci.MaskRegister |= (1 << _pci.Pin); // enable pci on specified pin
}

uint16_t Max31820::readTemperature()
{
    /* must disable the pin change interrupt before writing 
       or we'll inadvertantly trigger lots of interrupts on the port */
    _pci.MaskRegister &= ~(1 << _pci.Pin);
    oneWire.reset();
    if(oneWire.devicePresent())
    {
        oneWire.write(CMD_SKIP_ROM);
        oneWire.write(CMD_READ_SCRATCHPAD);

        /* max31820 sends LSB first and avr is little endian*/
        uint8_t temp[2];
        temp[0] = oneWire.read();
        temp[1] = oneWire.read();

        _state = Max31820State::RESET;
        /* Convert byte arrary to int by casting to int pointer then dereferencing */
        return *((uint16_t*)temp); 
    }

    return 0;
}

Max31820State Max31820::state()
{
    return _state;
}