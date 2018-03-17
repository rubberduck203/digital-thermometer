/*
    Test program for fuzzing the SevenSegment Display subsystem.
*/

#include <iostream>
#include <string>

#include "../src/SevenSegmentDisplay.h"
#include "../src/Max31820.h"

int main() {
    int oneWirePin;
    std::cin >> std::hex >> oneWirePin;

    IOPort_t oneWirePort{};
    OneWireImpl impl(oneWirePort, oneWirePin);
    OneWire oneWire(impl);

    volatile uint8_t reg = oneWirePin;
    PinChangeInterrupt_t pci(reg, oneWirePin, reg, oneWirePin);

    Max31820 tempSensor(oneWire, pci);

    IOPort_t shiftRegisterPort{};
    ShiftRegister shiftRegister(shiftRegisterPort);
    SevenSegment sevenSeg(shiftRegister);
    SevenSegmentDisplay display(sevenSeg);

    uint16_t input;
    std::cin >> std::hex >> input;
    //std::cout << std::hex << input << std::endl;

    int scaleIn;
    std::cin >> scaleIn;
    SevenSegmentDisplay::Scale scale = scaleIn ? SevenSegmentDisplay::Farenheit : SevenSegmentDisplay::Celcius;

    tempSensor.state();
    tempSensor.requestTemperature();
    tempSensor.readTemperature();
    display.write(input, scale);

    return 0;
}