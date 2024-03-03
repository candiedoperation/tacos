/*
    tacOS
    Copyright (C) 2023  Atheesh Thirumalairajan

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

#include <drivers/video/vga.hpp>
#include <kernel/types.hpp>

using namespace tacos::Drivers::Video;
using namespace tacos::Kernel;

/* Initialize the Static VGA Buffer Addresses */
u8* VgaTextMode::MemoryAddress = (u8*)0xB8000;
u8 VgaTextMode::ScreenWidth = 80;
u8 VgaTextMode::ScreenHeight = 25;
u16 VgaTextMode::CursorPos = 0;

void VgaTextMode::BufferWrite(char* buffer)
{
    /* Use Default Colors */
    BufferWrite(buffer, vga_color::WHITE, vga_color::BLACK);
}

void VgaTextMode::BufferWrite(char* buffer, vga_color FgColor, vga_color BgColor)
{
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            /* Set Cursor Positions for Next Line */
            int currentLine = ((int)(CursorPos / ScreenWidth)) + 1;
            CursorPos = (currentLine * (ScreenWidth));

            /* Don't Print \n */
            continue;
        }

        u8 TextProperties = ((u8)BgColor << 4) | ((u8)FgColor);
        MemoryAddress[CursorPos] = buffer[i];
        MemoryAddress[CursorPos + 1] = TextProperties;

        /* Move Memory Position */
        CursorPos += 2;
    }
}