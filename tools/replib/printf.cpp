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

#include <kernel/types.hpp>
#include <drivers/video/vga.hpp>
#include <tools/replib/printf.hpp>

using namespace tacOS::Tools;
using namespace tacOS::Drivers::Video;

/// @brief Draft printf Function (u64 Only)
/// @param Num u64 Integer
void RepLib::printf(u64 Num, int Base) {
    char* map = "0123456789ABCDEF";
    char tmp[21];
    tmp[20] = '\0';
    u8 i = sizeof(tmp);

    do {
        tmp[--i] = map[Num % Base]; /* FUTURE: Invalid Base will cause UNINTENDED MEM ACCESS! */
        Num /= Base;
    } while (Num != 0);

    /* Print The String */
    VgaTextMode::BufferWrite(tmp + i);
}

/// @brief Draft printf Function for Strings
/// @param Str String to Print
void RepLib::printf(char* Str) {
    /* Future Might Include Glyphs, Etc? */
    VgaTextMode::BufferWrite(Str);
}