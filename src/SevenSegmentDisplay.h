#ifndef _SEVEN_SEG_DISPLAY_H_
#define  _SEVEN_SEG_DISPLAY_H_

#include <stdint.h>
#include "SevenSegment.h"

// TODO: Extract Display interface so we can implement other displays
class SevenSegmentDisplay
{ 
private:
    SevenSegment& driver;

public:
    enum Scale 
    {
        Celcius,
        Farenheit
    };

    SevenSegmentDisplay(SevenSegment& driver);
    virtual void write(uint16_t temp, Scale scale);
};

#endif