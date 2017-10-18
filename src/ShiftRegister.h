#include <stdint.h>

struct IOPort_t
{
    uint8_t Direction;
    uint8_t Data;
} ;

class ShiftRegister
{
public:
    ShiftRegister(IOPort_t& port);
    void writeByte(uint8_t data);
};