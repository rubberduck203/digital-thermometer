#ifndef _MAX31820_H_
#define _MAX31820_H_

#include "OneWire.h"

class Max31820 
{
private:
    OneWire &oneWire;

public:
    Max31820(OneWire &oneWire);
    void requestTemperature();
};

#endif