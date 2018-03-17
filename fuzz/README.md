# Fuzzer

This directory holds the necessary code to fuzz the project.

## Running the Fuzzer

```bash
make
afl-fuzz -i in -o out bin/main
```

## Seed Data

The seed data is stored in the `in` directory.
Each file contains four numbers.
These correspond to:

 - OneWire Pin and PinChangeInterrupt Params (hex)
   - Should probably be split up into individual args
 - Data to be read from the one wire pin by FakeOneWireImpl
   - An integer that is treated as a string of bits
 - UInt16 of data to send to the display
 - 0/1 boolean that indicated Celsius or Fahrenheit to be displayed