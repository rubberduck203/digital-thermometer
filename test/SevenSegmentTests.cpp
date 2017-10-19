#include <stdint.h>
#include "CppUTest/TestHarness.h"

TEST_GROUP(SevenSegDriverTests)
{
};

IGNORE_TEST(SevenSegDriverTests, applesauce)
{
    FAIL("boom");
}


// Driven by 3 shift registers.
// 3 bytes of data is shifted out

// First shift register is the 10s digit (last byte shifted in)
// Second shift register is the 1s digit (second byte shifted in)
// Third (last) shift register is the inverted display for °C/°F
