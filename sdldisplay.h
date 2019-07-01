#ifndef __CHIP8_SDL_DISPLAY_H__
#define __CHIP8_SDL_DISPLAY_H__

#include "display.h"
#include <SDL2/SDL.h>

#define DISPLAY_SCREEN_WIDTH 640
#define DISPLAY_SCREEN_HEIGHT 320

/**
 * Implementation of Display using the SDL2 library
 */
class SDLDisplay: public Display
{
    private:
        SDL_Window* window;

    public:
        SDLDisplay();
        void render(unsigned char *screenBuffer);
        ~SDLDisplay();
};

#endif
