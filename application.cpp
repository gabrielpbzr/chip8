#include "application.h"

Application::Application(Display* display, Chip8* cpu)
{    
    this->display = display;
    this->cpu = cpu;
    this->quit = false;
}

void Application::run()
{
    this->cpu->init();
    while (!this->quit) {
        this->tick();
        usleep(160000);
    }

}

void Application::tick()
{
    this->cpu->execute();
}

bool Application::loadRom(const char* rom)
{
    return this->cpu->load(rom);
}

void Application::exit()
{

}

Application::~Application()
{
    display = NULL;
    cpu = NULL;
}