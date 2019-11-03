#include "application.h"
#include <unordered_map>

#define CLOCK_DELAY 80000 //Microseconds

std::unordered_map<int, int> keyboard{
    {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
    {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
    {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
    {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}
};

Application::Application(Display* display, Chip8* cpu)
{    
    this->display = display;
    this->cpu = cpu;
    this->quit = false;
    memset(this->keys, 0, KEY_COUNT);
}

void Application::run()
{
    this->cpu->init();
    this->display->init();
    SDL_Init(SDL_INIT_EVENTS);
    while (!this->quit) {        
        this->processEvents();
        this->tick();
        usleep(CLOCK_DELAY);
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
        switch (event.type) {
            case SDL_QUIT:
                this->exit();
            break;
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                auto i = keyboard.find(event.key.keysym.sym);
                if (i == keyboard.end())
                    break;
                unsigned char key = i->second;
                this->cpu->setKeyState(key, event.type == SDL_KEYDOWN);
                if (event.type == SDL_KEYDOWN && this->cpu->isWaitingKey()) {
                    this->cpu->setKeyState(key, event.type == SDL_KEYDOWN);
                    this->cpu->stopWaitingKey();
                }
                printf("KEY PRESSED => %x\n", i->second);
                break;
            }
        }
    }
}

Application::~Application()
{
    display = NULL;
    cpu = NULL;
}