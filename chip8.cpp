#include "chip8.h"
#include <fstream>
#include <iostream>
#include <cstdio>

void Chip8::init()
{
    this->I = 0x0;
    this->sp = 0x0;
    this->pc = 0x0;
}

void Chip8::load(const char* file)
{
    std::ifstream in;
    in.open(file, std::ios::binary);
    int offset = 0;
    while (!in.eof()) {
        char ch = 0;
        in.read(&ch, 1);
        memory[MEMORY_START_ADDRESS + offset] = (unsigned char) ch;
        offset++;
    }
    in.close();
}

void Chip8::execute()
{
    //fetch
    //decode
    //execute
}

void Chip8::dump()
{
    printf("========================= MEMORY DUMP =============================\n");
    for (unsigned short i = MEMORY_START_ADDRESS; i < MEMORY_SIZE; i++) {
        if ((i % 16) == 0) {
            printf("\n");
        }
        printf("%02x ", memory[i]);
    }
    printf("\n");
    printf("========================= MEMORY DUMP =============================\n");
}