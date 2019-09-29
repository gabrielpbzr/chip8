#ifndef __CHIP8_KEYBOARD_LISTENER_H__
#define __CHIP8_KEYBOARD_LISTENER_H__

#include <stdint.h>

namespace Chip8
{
   /**
    * Interface for a keyboard listener
    * @author Gabriel P. Bezerra <gabrielpbzr@gmail.com>
    */
    class KeyboardListener
    {
    public:
        virtual void onKeyUp(uint8_t key) = 0;
        virtual void onKeyDown(uint8_t key) = 0;
    };
} // namespace Chip8
#endif
