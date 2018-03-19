/*
    Test program for fuzzing the SevenSegment Display subsystem.
*/

#include <iostream>
#include <string>
#include <memory>

#include "../src/SevenSegmentDisplay.h"
#include "../src/Max31820.h"
#include "FakeOneWireImpl.h"

std::unique_ptr<Max31820> tempSensor;
std::unique_ptr<FakeOneWireImpl> impl;

void PCINT2_vect()
{
    tempSensor->_state = impl->readPin() ? Max31820State::READY_TO_READ : Max31820State::RESET;
}

int main() {
    int oneWirePin;
    std::cin >> std::hex >> oneWirePin;

    long data;
    std::cin >> data;

    int scaleIn;
    std::cin >> scaleIn;

    IOPort_t oneWirePort{};
    impl = std::make_unique<FakeOneWireImpl>(oneWirePort, oneWirePin, data);
    OneWire oneWire(*impl);

    volatile uint8_t reg = oneWirePin;
    PinChangeInterrupt_t pci(reg, oneWirePin, reg, oneWirePin);

    tempSensor = std::make_unique<Max31820>(oneWire, pci);

    IOPort_t shiftRegisterPort{};
    ShiftRegister shiftRegister(shiftRegisterPort);
    SevenSegment sevenSeg(shiftRegister);
    SevenSegmentDisplay display(sevenSeg);

    SevenSegmentDisplay::Scale scale = scaleIn ? SevenSegmentDisplay::Farenheit : SevenSegmentDisplay::Celcius;

    //simulate an interupt
    PCINT2_vect();

    switch(tempSensor->state())
    {
        case Max31820State::RESET:
            tempSensor->requestTemperature();
            break;
        case Max31820State::WAITING:
            //_delay_ms(10);
            break;
        case Max31820State::READY_TO_READ:
            uint16_t temperature = tempSensor->readTemperature();
            display.write(temperature, scale);
            break;
    }

    return 0;
}