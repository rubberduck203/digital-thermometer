# name of the resulting program/hex file
PROGNAME = demo
# Valid avr-gcc device from this list: 
# http://www.atmel.com/webdoc/avrlibcreferencemanual/index_1supp_devices.html
DEVICE ?= atmega328
# Processor speed (check your datasheet)
F_CPU ?= 16000000UL

### Shouldn't need to modify below this line ###
CC = avr-gcc
CXX = avr-g++
SRC = ./
OBJ = obj
BIN = bin

CPPFLAGS = -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -std=c++11

all: $(PROGNAME)
$(PROGNAME): $(BIN)/$(PROGNAME).hex size

include ../rules.mk

# hex <- elf
$(BIN)/%.hex: $(BIN)/%.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

# elf <- [obj]
objects := $(patsubst %.c,$(OBJ)/%.o,$(wildcard *.c*))
objects := $(patsubst %.cpp, $(OBJ)/%.o, $(objects))

.PRECIOUS: $(BIN)/%.elf
$(BIN)/%.elf: $(objects)
	@mkdir -p $(BIN)
	$(CC) $(CPPFLAGS) -Wl,-Map,$(BIN)/$(PROGNAME).map $^ -o $@

.PHONY: size
size:
	avr-size --format=avr --mcu=$(DEVICE) $(BIN)/$(PROGNAME).elf
