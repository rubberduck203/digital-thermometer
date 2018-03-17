/*
    Test program for fuzzing the SevenSegment Display subsystem.
*/

#include <iostream>
#include <string>

#include "../src/SevenSegmentDisplay.h"

int main() {
    IOPort_t shiftRegisterPort{};
    ShiftRegister shiftRegister(shiftRegisterPort);
    SevenSegment sevenSeg(shiftRegister);
    SevenSegmentDisplay display(sevenSeg);

    uint16_t input;
    std::cin >> std::hex >> input;
    //std::cout << std::hex << input << std::endl;
    display.write(input, SevenSegmentDisplay::Scale::Celcius);

    return 0;
}