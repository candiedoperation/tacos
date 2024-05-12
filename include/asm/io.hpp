/*
    tacOS
    Copyright (C) 2024  Atheesh Thirumalairajan

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ASM_IO_HPP
#define ASM_IO_HPP

#include <kernel/types.hpp>

using namespace tacOS::Kernel;

namespace tacOS {
namespace ASM {
    /// @brief Contains x86 Assembly Helpers Interrupt Operations
    class IO {
    public:
        /*
            Inline functions are defined in the header file as they
            expand at the location they're called. This makes reduce
            the call times significantly when they're rapidly called.
        */

        /// @brief Sends a Byte as an output to a port
        /// @param port Defines the Output Port
        /// @param output Defines the Output Message (1 byte)
        static inline void outb(u16 port, u8 output)
        {
            __asm__ volatile(
                "outb %b0, %w1" // Assembly instruction to perform output
                : /* Output operands (none in this case) */
                : "a"(output), "Nd"(port) /* Input operands: "a" indicates eax, "N" picks a suitable register */
                : "memory" /* Informs the compiler about potential memory changes, Prevents optimizations */
            );
        }

        /// @brief Recevies a Byte as input from a port
        /// @param port Defines Input Port
        /// @return Byte read from the Port
        static inline u8 inb(u16 port)
        {
            u8 input;
            __asm__ volatile(
                "inb %w1, %b0" /* Assembly instruction to perform input */
                : "=a"(input) /* Output operand, Uses eax, sets to input */
                : "Nd"(port) /* Input operand */
                : "memory" /* Informs the compiler about potential memory changes */
            );

            /* Return Received Value */
            return input;
        }

        /// @brief Delay Timer that takes 1 to 4 microseconds.
        static inline void wait()
        {
            /* Writes Gibberish to a port 0x80 */
            outb(0x80, 0);
        }
    };
}
} // namespace tacOS

#endif