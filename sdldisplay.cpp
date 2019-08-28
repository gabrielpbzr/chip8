#include "sdldisplay.h"
#include "chip8.h"

#define PIXEL_SIZE 10

void SDLDisplay::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Cannot init %s", SDL_GetError());
        return;
    }
    this->window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    this->renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    SDL_Delay(3000);
}

void SDLDisplay::render(unsigned char *screenBuffer)
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    SDL_RenderClear(this->renderer);
    
    for (unsigned int i = 0; i < SCREEN_SIZE; i++) {
        if (screenBuffer[i] == 0) {
            SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
        } else {
            SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        }
        SDL_Rect rect;
            rect.x = (i % 64) * PIXEL_SIZE;
            rect.y = ((int) i / 64) * PIXEL_SIZE;
            rect.h = PIXEL_SIZE;
            rect.w = PIXEL_SIZE;
            SDL_RenderFillRect(this->renderer, &rect);
    }

    SDL_RenderPresent(this->renderer);
}

SDLDisplay::~SDLDisplay()
{
    renderer = NULL;
    window = NULL;
}