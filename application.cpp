#include "application.h"
#include <SDL2/SDL.h>

Application::Application(Display* display, Chip8* cpu)
{    
    this->display = display;
    this->cpu = cpu;
    this->quit = false;
}

void Application::run()
{
    this->cpu->init();
    this->display->init();
    SDL_Init(SDL_INIT_EVENTS);
    while (!this->quit) {        
        this->tick();
        this->processEvents();
        usleep(160000);
    }

}

void Application::tick()
{
    this->cpu->execute();
    if (cpu->needsDrawing()) {
        display->render(cpu->getScreen());
    }
}

bool Application::loadRom(const char* rom)
{
    return this->cpu->load(rom);
}

void Application::exit()
{
    this->quit = true;
}

void Application::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            this->exit();
        }
    }
}

Application::~Application()
{
    display = NULL;
    cpu = NULL;
}