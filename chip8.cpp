#include "chip8.h"
#include <fstream>
#include <iostream>
#include <cstdio>

void Chip8::init()
{
    this->I = 0x0;
    this->sp = 0x0;
    this->pc = 0x200;
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
    unsigned short opcode = this->fetchInstruction();
    if (opcode == 0x0000) {
        return;
    }
    printf("OPCODE => %04x\n", opcode);
    // decode
    unsigned char family = (unsigned char)((opcode & 0xF000) >> 12);
    // execute
    switch (family) {
        case 0x0:
            switch (opcode & 0x00FF) {
                case 0xE0:
                    printf("CLEAR DISPLAY\n");
                    break;
                case 0xEE:
                    returnFromSubRoutine();
                    break;
            }
            break;
        case 0x1:
            this->jumpToAddress(opcode & 0x0FFF);
            break;
        case 0x2:
            this->callSubroutineAt(opcode & 0xFFF);
            break;
        case 0x3:
            // NOP
            break;
        case 0x4:
            // NOP
            break;
        case 0x5:
            // NOP
            break;
        case 0x6:
            // NOP
            break;
        case 0x7:
            // NOP
            break;
        case 0x8:
            // NOP
            break;
        case 0x9:
            // NOP
            break;
        case 0xA:
            // NOP
            break;
        case 0xB:
            // NOP
            break;
        case 0xC:
            // NOP
            break;
        case 0xD:
            // NOP
            break;
        case 0xE:
            // NOP
            break;
        case 0xF:
            // NOP
            break;
        default:
            // RAISE AN ERROR: OPCODE NOT SUPPORTED
            break;
    }
}

unsigned short Chip8::fetchInstruction()
{
    unsigned short instruction = (unsigned short) ((memory[this->pc] << 8) | memory[this->pc + 1]);
    if (this->pc < MEMORY_SIZE) {
        this->pc += 2;
    }
    
    return instruction;
}

void Chip8::dump()
{
    printf("============================== MEMORY DUMP ===================================\n");
    for (unsigned short i = MEMORY_START_ADDRESS; i < MEMORY_SIZE; i++) {
        if ((i % 16) == 0) {
            printf("\n");
        }
        printf("%02x ", memory[i]);
    }
    printf("\n");
    printf("============================== MEMORY DUMP ===================================\n");
}

void Chip8::jumpToAddress(unsigned short address)
{
    printf("GOTO ADDRESS 0x%x\n", address);
    this->pc = address;
}

void Chip8::returnFromSubRoutine() {
    this->pc = this->stack[this->sp];
    this->sp--;
    printf("RETURN FROM ROUTINE TO 0x%04x\n", this->pc);
}

void Chip8::callSubroutineAt(unsigned short address) {
    this->stack[this->sp] = this->pc;
    this->sp++;
    this->pc = address;
    printf("CALLING ROUTINE AT 0x%04x\n", this->pc);
}