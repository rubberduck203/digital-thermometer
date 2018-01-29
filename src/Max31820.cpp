#include "Max31820.h"

Max31820::Max31820(OneWire &oneWire): oneWire(oneWire) { }

void Max31820::requestTemperature()
{
    oneWire.reset();
}