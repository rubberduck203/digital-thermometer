/*
    Test program for fuzzing the SevenSegment Display subsystem.
*/

#include <iostream>
#include <string>

#include "../src/SevenSegmentDisplay.h"
#include "../src/Max31820.h"
#include "FakeOneWireImpl.h"

int main() {
    int oneWirePin;
    std::cin >> std::hex >> oneWirePin;

    int data;
    std::cin >> data;

    uint16_t input;
    std::cin >> std::hex >> input;

    int scaleIn;
    std::cin >> scaleIn;

    IOPort_t oneWirePort{};
    FakeOneWireImpl impl(oneWirePort, oneWirePin, data);
    OneWire oneWire(impl);

    volatile uint8_t reg = oneWirePin;
    PinChangeInterrupt_t pci(reg, oneWirePin, reg, oneWirePin);

    Max31820 tempSensor(oneWire, pci);

    IOPort_t shiftRegisterPort{};
    ShiftRegister shiftRegister(shiftRegisterPort);
    SevenSegment sevenSeg(shiftRegister);
    SevenSegmentDisplay display(sevenSeg);

    SevenSegmentDisplay::Scale scale = scaleIn ? SevenSegmentDisplay::Farenheit : SevenSegmentDisplay::Celcius;

    tempSensor.state();
    tempSensor.requestTemperature();
    tempSensor.readTemperature();
    display.write(input, scale);

    return 0;
}