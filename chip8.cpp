#include "chip8.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

void Chip8::init()
{
    this->I = 0x0;
    this->sp = 0x0;
    this->pc = 0x200;
    memset(this->V, 0, 16);
}

void Chip8::load(const char *file)
{
    std::ifstream in;
    in.open(file, std::ios::binary);
    int offset = 0;
    while (!in.eof())
    {
        char ch = 0;
        in.read(&ch, 1);
        memory[MEMORY_START_ADDRESS + offset] = (unsigned char)ch;
        offset++;
    }
    in.close();
}

void Chip8::execute()
{
    //fetch
    unsigned short opcode = this->fetchInstruction();
    if (opcode == 0x0000)
    {
        return;
    }
    printf("OPCODE => %04x\n", opcode);
    // decode
    unsigned char family = (unsigned char)((opcode & 0xF000) >> 12);
    // execute
    switch (family)
    {
        case 0x0:
            switch (opcode & 0x00FF)
            {
            case 0xE0:
                printf("CLEAR DISPLAY\n");
                break;
            case 0xEE:
                returnFromSubRoutine();
                break;
            }
            break;
        case 0x1:
        {
            unsigned short address = opcode & 0x0FFF;
            this->jumpToAddress(address);
            break;
        }
        case 0x2:
        {
            unsigned short address = opcode & 0x0FFF;
            this->callSubroutineAt(address);
            break;
        }
        case 0x3:
        {
            unsigned short rx = ((opcode & 0x0F00) >> 8);
            unsigned char value = (opcode & 0x00FF);
            this->skipNextInstructionIfRegisterValueEquals(rx, value);
            break;
        }
        case 0x4:
        {
            unsigned short rx = ((opcode & 0x0F00) >> 8);
            unsigned char value = (opcode & 0x00FF);
            this->skipNextInstructionIfRegisterValueNotEquals(rx, value);
            break;
        }
        case 0x5:
        {
            unsigned char rx = ((opcode & 0x0F00) >> 8);
            unsigned char ry = ((opcode & 0x00F0) >> 4);
            this->skipNextInstructionIfEquals(rx, ry);
            break;
        }
        case 0x6:
        {
            unsigned char rx = ((opcode & 0x0F00) >> 8);
            unsigned char value = (opcode & 0x00FF);
            this->setRegisterValue(rx, value);
            break;
        }
        case 0x7:
        {
            unsigned char rx = ((opcode & 0x0F00) >> 8);
            unsigned char value = (opcode & 0x00FF);
            this->addToRegisterValue(rx, value);
            break;
        }

        case 0x8:
        {
            unsigned char op = (opcode & 0x000F);
            unsigned char x = (opcode & 0x0F00) >> 8;
            unsigned char y = (opcode & 0x00F0) >> 4;
            switch (op)
            {
                case 0x00:
                    this->V[x] = this->V[y];
                    break;
                case 0x01:
                    this->V[x] = this->V[x] | this->V[y];
                    break;
                case 0x02:
                    this->V[x] = this->V[x] & this->V[y];
                    break;
                case 0x03:
                    this->V[x] = this->V[x] ^ this->V[y];
                    break;
                case 0x04:
                    this->V[x] += this->V[y];
                    break;
                case 0x05:
                    this->V[x] -= this->V[y];
                    break;
                case 0x06:
                    this->V[0x0F] = (this->V[x] & 0x01);
                    this->V[x] >>= 1;
                    break;
                case 0x07:
                    this->V[x] = this->V[y] - this->V[x];
                    break;
                case 0x0E:
                    this->V[0x0F] = (this->V[x] & 0x80);
                    this->V[x] <<= 1;
                    break;
            }
            break;
        }
        case 0x9:
        {
            unsigned char rx = ((opcode & 0x0F00) >> 8);
            unsigned char ry = ((opcode & 0x00F0) >> 4);
            this->skipNextInstructionIfNotEquals(rx, ry);
            break;
        }
        case 0xA:
            this->I = (char)(opcode & 0x0FFF);
            break;
        case 0xB:
        {
            unsigned short address = (unsigned short)(opcode & 0x0FFF);
            this->jumpToAddressPlusV0(address);
            break;
        }
        case 0xC:
        {
            unsigned char x = (opcode & 0x0F00);
            unsigned char value = (opcode & 0x00FF);
            this->V[x] = randomByte() && value;
            break;
        }
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
    unsigned short instruction = (unsigned short)((memory[this->pc] << 8) | memory[this->pc + 1]);
    if (this->pc < MEMORY_SIZE)
    {
        this->pc += 2;
    }

    return instruction;
}

void Chip8::dump()
{
    printf("============================== MEMORY DUMP ===================================\n");
    for (unsigned short i = MEMORY_START_ADDRESS; i < MEMORY_SIZE; i++)
    {
        if ((i % 16) == 0)
        {
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

void Chip8::jumpToAddressPlusV0(unsigned short address)
{
    unsigned short newAddress = address + this->V[0];
    printf("GOTO ADDRESS 0x%x\n PLUS V0 CONTENT => 0x03%x", address, newAddress);
    this->pc = newAddress;
}

void Chip8::returnFromSubRoutine()
{
    this->pc = this->stack[this->sp];
    this->sp--;
    printf("RETURN FROM ROUTINE TO 0x%04x\n", this->pc);
}

void Chip8::callSubroutineAt(unsigned short address)
{
    this->stack[this->sp] = this->pc;
    this->sp++;
    this->pc = address;
    printf("CALLING ROUTINE AT 0x%04x\n", this->pc);
}

void Chip8::skipNextInstructionIfRegisterValueEquals(unsigned char registerIndex, unsigned char value)
{
    printf("SKIP INSTRUCTION IF V[%d] == %d: %d\n", registerIndex, value, this->V[registerIndex]);
    if (this->V[registerIndex] == value)
    {
        printf("\tSKIPPED\n");
        this->pc += 2;
    }
}

void Chip8::skipNextInstructionIfRegisterValueNotEquals(unsigned char registerIndex, unsigned char value)
{
    printf("SKIP INSTRUCTION IF V[%d] != %d: %d\n", registerIndex, value, this->V[registerIndex]);
    if (this->V[registerIndex] == value)
    {
        printf("\tSKIPPED\n");
        this->pc += 2;
    }
}

void Chip8::skipNextInstructionIfEquals(unsigned char registerIndex, unsigned char otherRegisterIndex)
{
    printf("SKIP INSTRUCTION IF V[%d] == V[%d]: %d == %d\n", registerIndex, otherRegisterIndex, this->V[registerIndex], this->V[otherRegisterIndex]);
    if (this->V[registerIndex] == this->V[otherRegisterIndex])
    {
        printf("\tSKIPPED\n");
        this->pc += 2;
    }
}

void Chip8::skipNextInstructionIfNotEquals(unsigned char registerIndex, unsigned char otherRegisterIndex)
{
    printf("SKIP INSTRUCTION IF V[%d] != V[%d]: %d == %d\n", registerIndex, otherRegisterIndex, this->V[registerIndex], this->V[otherRegisterIndex]);
    if (this->V[registerIndex] != this->V[otherRegisterIndex])
    {
        printf("\tSKIPPED\n");
        this->pc += 2;
    }
}

void Chip8::setRegisterValue(unsigned char registerIndex, unsigned char value)
{
    this->V[registerIndex] = value;
    printf("ASSIGN VALUE TO V[%d] = 0x%02x\n", registerIndex, value);
}

void Chip8::addToRegisterValue(unsigned char registerIndex, unsigned char value)
{
    this->V[registerIndex] += value;
    printf("ADD VALUE TO V[%d] = 0x%02x\n", registerIndex, value);
}

unsigned char randomByte() {
    float rand = ((float) random()) / RAND_MAX;
    return (unsigned char) (rand * 0xFF);
}