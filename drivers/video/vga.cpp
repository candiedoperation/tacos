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

VgaTextMode::VgaTextMode()
{
    Buffer.MemoryAddress = (Byte *)0xB8000;
    Buffer.ScreenWidth = 80;
    Buffer.ScreenHeight = 25;
    Buffer.CursorPos = 0;
}

void VgaTextMode::BufferWrite(char *buffer)
{
    /* Use Default Colors */
    BufferWrite(buffer, VgaColor::WHITE, VgaColor::BLACK);
}

void VgaTextMode::BufferWrite(char *buffer, VgaColor FgColor, VgaColor BgColor)
{
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        Byte TextProperties = ((Byte) BgColor << 4) | ((Byte) FgColor);
        Buffer.MemoryAddress[Buffer.CursorPos] = buffer[i];
        Buffer.MemoryAddress[Buffer.CursorPos + 1] = TextProperties;

        /* Move Memory Position */
        Buffer.CursorPos += 2;
    }
}