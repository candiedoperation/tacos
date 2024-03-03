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

using namespace tacos::Kernel;

namespace tacos {
namespace ASM {
    /// @brief Contains x86 Assembly Helpers Interrupt Operations
    class IO {
    public:
        static inline void outb(u16 port, u8 output);
        static inline u8 inb(u16 port);
        static inline void wait();
    };
}
} // namespace tacos

#endif