#include <iostream>

#include "application.h"
#include "sdldisplay.h"

int main(int argc, char** argv)
{
    Display *display = new SDLDisplay(); //TODO Implementar aqui um objeto para renderização dos gráficos
    
    Chip8 ch8;
    
    Application app(display, &ch8);

    if (argv[1] == NULL) {
        std::cout << "ROM file not found" << std::endl;
        return 1;
    }
    char* rom = argv[1];
    std::cout << "Selected rom: " << argv[1] << std::endl;

    if (!app.loadRom(rom)) {
        std::cout << "ROM file not found" << std::endl;
        return 1;
    }

    app.run();

    return 0;
}
