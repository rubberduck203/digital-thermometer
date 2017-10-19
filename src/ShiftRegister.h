#include <stdint.h>

struct IOPort_t
{
    uint8_t Direction;
    uint8_t Data;
} ;

class ShiftRegister
{
private:
    IOPort_t& port;
public:
    ShiftRegister(IOPort_t& port);
    void writeByte(uint8_t data);
    void writeBit(uint8_t value);
    void latch();
};