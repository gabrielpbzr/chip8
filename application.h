#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <unistd.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "chip8.h"

class Application
{
    private:
        Display* display;
        Chip8* cpu;
        bool quit;
        unsigned char keys[KEY_COUNT];
    protected:
        void tick(void);
        void handleKeyDown(SDL_Keycode key);
        void handleKeyUp(SDL_Keycode key);
    public:
        Application(Display* display, Chip8* cpu);
        void run(void);
        bool loadRom(const char* rom);
        void exit(void);
        void processEvents(void);
        ~Application(void);

};

#endif