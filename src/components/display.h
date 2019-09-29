#ifndef __CHIP8_DISPLAY_H__
#define __CHIP8_DISPLAY_H__

#include <stdint.h>

namespace Chip8
{
   /**
    * Interface for a display implementation
    * @author Gabriel P. Bezerra <gabrielpbzr@gmail.com>
    */
    class Display
    {
    public:
        virtual void init() = 0;
        virtual void render(uint8_t* screenBuffer) = 0;
        virtual uint8_t getPixelValue(uint32_t x, uint32_t y) = 0;
        virtual void setPixelValue(uint32_t x, uint32_t y, uint8_t value) = 0;
    };
} // namespace Chip8
#endif
