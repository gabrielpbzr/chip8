#ifndef __DISPLAY_H__
#define __DISPLAY_H__

/**
 * Interface for a display implementation
 * @author Gabriel P. Bezerra <gabrielpbzr@gmail.com>
 */
class Display
{
    public:
        virtual void render(unsigned char *screenBuffer) = 0;
};

#endif