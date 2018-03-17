# Fuzzer

This directory holds the necessary code to fuzz the project.
It currently tests the display subsystem in Celcius mode.

## Running the Fuzzer

```bash
make
afl-fuzz -i in -o out bin/main
```