#ifndef __CHIP8_DUMPABLE_H__
#define __CHIP8_DUMPABLE_H__

#include <iostream>;

namespace Chip8
{
   /**
    * Interface for objects which dump its contents implementation
    * @author Gabriel P. Bezerra <gabrielpbzr@gmail.com>
    */
    class Dumpable
    {
        public:
            /**
             * Dumps its contents to the output stream
             * 
             * @param std::ostream output stream
             */
            virtual void dump(std::ostream* out) = 0;
    };
} // namespace Chip8
#endif
