#include "Max31820.h"

const uint8_t CMD_SKIP_ROM = 0xCC;
const uint8_t CMD_CONVERTT = 0x44;

Max31820::Max31820(OneWire &oneWire): oneWire(oneWire) { }

void Max31820::requestTemperature()
{
    oneWire.reset();
    oneWire.devicePresent();
    oneWire.write(CMD_SKIP_ROM);
    oneWire.write(CMD_CONVERTT);

    oneWire.issueReadSlot();
}