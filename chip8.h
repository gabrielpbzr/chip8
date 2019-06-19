#ifndef __CHIP8__
#define __CHIP8__

#define MEMORY_SIZE 0x1000
#define MEMORY_START_ADDRESS 0x200
#define REGISTER_COUNT 0x10
#define SCREEN_SIZE 64 * 32
#define STACK_SIZE 16

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
    
    public:
        void init();
        void load(const char* file);
        void execute();
        void dump();
};
#endif