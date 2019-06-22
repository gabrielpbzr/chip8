#include <iostream>
#include <unistd.h>
#include "chip8.h"

int main(int argc, char** argv)
{
    Chip8 ch8;
    ch8.init();
    if (argv[1] == NULL) {
        std::cout << "ROM file not found" << std::endl;
        return 1;
    }
    char* rom = argv[1];
    std::cout << "Selected rom: " << argv[1] << std::endl;

    if (!ch8.load(rom)) {
        std::cout << "ROM file not found" << std::endl;
        return 1;
    }

    while (ch8.execute() == 0) {
        usleep(160000);
    }

    //ch8.dump();
    return 0;
}