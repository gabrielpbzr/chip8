#include <iostream>
#include "chip8.h"

int main(int argc, char** argv)
{
    Chip8 ch8;
    ch8.init();
    ch8.load("roms/pong2.c8");
    ch8.dump();
    return 0;
}