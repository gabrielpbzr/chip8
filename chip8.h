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
    
    protected:
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
        void load(const char* file);
        void execute();
        void dump();
};
#endif