#ifndef __SDLDISPLAY_H__
#define __SDLDISPLAY_H__
#include "display.h"
#include <SDL2/SDL.h>

class SDLDisplay : public Display {
    private:
        SDL_Window *window;
        SDL_Surface *surface;
        SDL_Renderer *renderer;
    public:
        void init();
        void render(unsigned char *screenBuffer);
        ~SDLDisplay();
};

#endif