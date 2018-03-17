CC = afl-clang
CXX = afl-clang++
SRC = ../src
TEST = ../test
OBJ = obj
BIN = bin

CPPFLAGS += -Wall -I../test/mocks -std=c++11

# Build the test executable
testSource := $(wildcard *.cpp)

# Include all the source from src, except the entry point
srcFiles := $(filter-out $(SRC)/Demo.cpp, $(wildcard $(SRC)/*.c*))
objects := $(patsubst $(SRC)/%.c,$(OBJ)/%.o, $(srcFiles))
objects := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(objects))

all: $(BIN)/main

### Compile code under test
include ../rules.mk

### Create test runner executable
$(BIN)/main: $(objects) $(testSource)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@ $(LD_LIBRARIES)