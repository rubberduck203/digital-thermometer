# Fuzzer

This directory holds the necessary code to fuzz the project.

## Running the Fuzzer

```bash
make
afl-fuzz -i in -o out bin/main
```