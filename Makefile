CC= g++
SOURCES=*.cpp
TARGET= bin/chip8
LINKER_FLAGS= -lSDL2 -lc
COMPILER_FLAGS= -Wall -o $(TARGET) -std=c++11

all: clean build

clean:
	rm -rf *.o bin/

build:
	mkdir bin/
	$(CC) $(SOURCES) $(COMPILER_FLAGS) $(LINKER_FLAGS)
