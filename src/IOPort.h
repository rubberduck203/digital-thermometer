#ifndef _IOPORT_H_
#define _IOPORT_H_

#include <stdint.h>

struct IOPort_t
{
    volatile uint8_t Direction;
    volatile uint8_t Data;
};

#endif