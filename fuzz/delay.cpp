/*
    Host implementations of util/delay.h
*/

#include <util/delay.h>
#include <thread>
#include <chrono>
#include <cmath>

void _delay_ms(double __ms)
{
    int ms = std::round(__ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void _delay_us(double __us)
{
    int us = std::round(__us);
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}