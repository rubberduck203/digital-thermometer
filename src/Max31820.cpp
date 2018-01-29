#include "Max31820.h"

const uint8_t CMD_SKIP_ROM = 0xCC;
const uint8_t CMD_CONVERTT = 0x44;

Max31820::Max31820(OneWire &oneWire, PinChangeInterrupt &pci): oneWire(oneWire), _pci(pci)
{ 
    _state = Max31820State::RESET;
    _pci.MaskRegister = 0x00; // disable pin change interrupts for all pins on port
    _pci.ControlRegister |= (1 << _pci.ControlRegisterEnableIndex);
}

void Max31820::requestTemperature()
{
    oneWire.reset();
    oneWire.devicePresent();
    oneWire.write(CMD_SKIP_ROM);
    oneWire.write(CMD_CONVERTT);

    oneWire.issueReadSlot();
    _state = Max31820State::WAITING;
    _pci.MaskRegister |= (1 << _pci.Pin); // enable pci on specified pin
}

Max31820State Max31820::state()
{
    return _state;
}