CC= g++
SOURCES=*.cpp
TARGET= bin/chip8
LINKER_FLAGS= -lSDL2
COMPILER_FLAGS= -Wall -o $(TARGET)

all: clean build

clean:
	rm -rf *.o bin/

build:
	mkdir bin/
	$(CC) $(SOURCES) $(COMPILER_FLAGS) $(LINKER_FLAGS)
