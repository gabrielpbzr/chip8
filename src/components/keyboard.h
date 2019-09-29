#ifndef __CHIP8_KEYBOARD_H__
#define __CHIP8_KEYBOARD_H__

#include <stdint.h>
#include "keyboard-listener.h"

namespace Chip8
{
   /**
    * Interface for a keyboard implementation
    * @author Gabriel P. Bezerra <gabrielpbzr@gmail.com>
    */
    class Keyboard
    {
        public:
            virtual void addListener(KeyboardListener* listener) = 0;
            virtual void keyPressed(uint8_t key) = 0;
            virtual void keyReleased(uint8_t key) = 0;
    };
} // namespace Chip8
#endif
