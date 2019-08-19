#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <unistd.h>
#include "display.h"
#include "chip8.h"

class Application
{
    private:
        Display* display;
        Chip8* cpu;
        bool quit;
    protected:
        void tick(void);
    public:
        Application(Display* display, Chip8* cpu);
        void run(void);
        bool loadRom(const char* rom);
        void exit(void);
        ~Application(void);

};

#endif