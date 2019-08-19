#include "chip8.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

Chip8::Chip8()
{
}

void Chip8::init()
{
    this->I = 0x0;
    this->sp = 0x0;
    this->pc = 0x200;
    memset(this->V, 0, 16);
    memset(this->screen, 0, SCREEN_SIZE);
    this->draw = false;
}

int Chip8::load(const char *file)
{
    std::ifstream in;
    in.open(file, std::ios::binary);
    
    if (!in.is_open()) {
        return 0;
    }
    
    int offset = 0;
    while (!in.eof())
    {
        char ch = 0;
        in.read(&ch, 1);
        memory[MEMORY_START_ADDRESS + offset] = (unsigned char)ch;
        offset++;
    }
    in.close();
    return 1;
}

int Chip8::execute()
{
    // Clear drawing flag
    this->draw = false;
    //fetch
    unsigned short opcode = this->fetchInstruction();
    if (opcode == 0x0000)
    {
        return 0;
    }
    printf("OPCODE => %hx\n", opcode);
    // decode
    unsigned char family = (unsigned char)((opcode & 0xF000) >> 12);
    // execute
    switch (family)
    {
        case 0x0:
            switch (opcode & 0x00FF)
            {
            case 0xE0:
                //Clear screen
                memset(this->screen, 0, SCREEN_SIZE);
                this->draw = true;
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
                    this->V[0xF] = ((this->V[x] + this->V[y]) > 0xFF) ? 1 : 0;
                    this->V[x] += this->V[y];
                    break;
                case 0x05:
                    this->V[0xF] = (this->V[y] > this->V[x]) ? 0 : 1;
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
            this->I = (opcode & 0x0FFF);
            printf("Address stored at I => 0x%x\n", this->I);
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
        {
            // Draw a sprite at (VX, VY) with size (8, N) at I
            unsigned char x = this->V[(opcode & 0x0F00) >> 8];
            unsigned char y = this->V[(opcode & 0x00F0) >> 4];
            unsigned char height = (opcode & 0x000F);
            this->V[0xF] = 0;

            for (unsigned char yLine = 0; yLine < height; yLine++) {
                // Load from memory at I the pixel value
                int pixel = this->memory[this->I + yLine];
                for (unsigned char xLine = 0; xLine < SPRITE_WIDTH; xLine++) {
                    // Look at each bit of pixel data
                    unsigned char pixelSet = (pixel & (0x80 >> xLine));
                    // If current pixel is set...
                    if (pixelSet != 0) {
                        // Calculate the pixel index at screen memory
                        unsigned short pixelIndex = (x + xLine) + ((y + yLine) * SCREEN_HEIGHT);

                        /* 
                         * If the pixel at the position is already defined, set the V[0xF] to one
                         * indicating that we have a collision
                         */
                        if (this->screen[pixelIndex] == 1) {
                            this->V[0xF] = 1;
                        }
                        // Flip pixel value using XOR 
                        this->screen[pixelIndex] ^= 1;
                    }
                }
            }
            
            this->draw = true;
            break;
        }
        case 0xE:
            // NOP
            break;
        case 0xF:
            // NOP
            break;
        default:
            // RAISE AN ERROR: OPCODE NOT SUPPORTED
            printf("\aOPCODE NOT SUPPORTED!\n");
            return 1;
            break;
    }
    
    return 0;
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
    this->pc = this->stack[--this->sp];
    printf("RETURN FROM ROUTINE TO 0x%04x\n", this->pc);
}

void Chip8::callSubroutineAt(unsigned short address)
{
    this->stack[this->sp++] = this->pc;
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

unsigned char Chip8::randomByte()
{
    float rand = ((float) random()) / RAND_MAX;
    unsigned char randomByte = (unsigned char) (rand * 0xFF);
    printf("Random byte is: 0x%02x", randomByte);
    return randomByte;
}

const char* Chip8::screen(void)
{
    return this->screen;
}

bool Chip8::needsDrawing()
{
    return this->draw;
}

Chip8::~Chip8()
{
}