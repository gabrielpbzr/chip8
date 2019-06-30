#ifndef __CHIP8__
#define __CHIP8__

#define MEMORY_SIZE 0x1000
#define MEMORY_START_ADDRESS 0x200
#define REGISTER_COUNT 0x10
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT
#define STACK_SIZE 16
#define SPRITE_WIDTH 8

class Chip8
{
    private:
        unsigned char memory[MEMORY_SIZE];
        unsigned char V[REGISTER_COUNT];
        unsigned char screen[SCREEN_SIZE];
        unsigned short I;
        unsigned short pc;
        unsigned short stack[STACK_SIZE];
        unsigned short sp;
        bool draw;
        
        unsigned char randomByte();
        unsigned short fetchInstruction();
        void jumpToAddress(unsigned short address);
        void jumpToAddressPlusV0(unsigned short address);
        void callSubroutineAt(unsigned short address);
        void returnFromSubRoutine(void);
        void skipNextInstructionIfRegisterValueEquals(unsigned char registerIndex, unsigned char value);
        void skipNextInstructionIfRegisterValueNotEquals(unsigned char registerIndex, unsigned char value);
        void skipNextInstructionIfEquals(unsigned char registerIndex, unsigned char otherRegisterIndex);
        void skipNextInstructionIfNotEquals(unsigned char registerIndex, unsigned char otherRegisterIndex);
        void setRegisterValue(unsigned char registerIndex, unsigned char value);
        void addToRegisterValue(unsigned char registerIndex, unsigned char value);
    
    public:
        void init();
        int load(const char* file);
        int execute();
        void dump();
};
#endif