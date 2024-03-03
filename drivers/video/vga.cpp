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

#include <kernel/types.hpp>
#include <drivers/video/vga.hpp>

using namespace tacos::Drivers::Video;
using namespace tacos::Kernel;

/* Initialize the Static VGA Buffer Addresses */
BYTE *VgaTextMode::MemoryAddress = (BYTE *)0xB8000;
BYTE VgaTextMode::ScreenWidth = 80;
BYTE VgaTextMode::ScreenHeight = 25;
WORD VgaTextMode::CursorPos = 0;

void VgaTextMode::BufferWrite(char *buffer)
{
    /* Use Default Colors */
    BufferWrite(buffer, VgaColor::WHITE, VgaColor::BLACK);
}

void VgaTextMode::BufferWrite(char *buffer, VgaColor FgColor, VgaColor BgColor)
{
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '\n')
        {
            /* Set Cursor Positions for Next Line */
            int currentLine = ((int)(CursorPos / ScreenWidth)) + 1;
            CursorPos = (currentLine * (ScreenWidth));

            /* Don't Print \n */
            continue;
        }

        BYTE TextProperties = ((BYTE)BgColor << 4) | ((BYTE)FgColor);
        MemoryAddress[CursorPos] = buffer[i];
        MemoryAddress[CursorPos + 1] = TextProperties;

        /* Move Memory Position */
        CursorPos += 2;
    }
}