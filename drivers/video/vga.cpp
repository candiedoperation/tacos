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
u8* VgaTextMode::MemoryAddress = (u8*)VGATM_MEM_ADDRESS;
u16 VgaTextMode::CursorPos = 0;

/// @brief Handles rudimentary non-buffered page down scroll
void VgaTextMode::PageDown()
{
    /*
        Scroll Up, Copy lines up and make space at bottom.
    */

    int SecondLineOffset = VGATM_SCR_WIDTH * 2;
    for (int Offset = SecondLineOffset; Offset < VGATM_SCR_PIXELS * 2; Offset++) {
        MemoryAddress[Offset - SecondLineOffset] = MemoryAddress[Offset];
    }

    /* Clear the Last Line */
    int LastLineOffset = (VGATM_SCR_WIDTH * (VGATM_SCR_HEIGHT - 1)) * 2;
    for (int Offset = LastLineOffset; Offset < VGATM_SCR_PIXELS * 2; Offset++) {
        /* Clear Both Text and Metadata bits */
        MemoryAddress[Offset] = 0;
    }

    /* Set Cursor Position to Last Line */
    CursorPos = (VGATM_SCR_WIDTH * (VGATM_SCR_HEIGHT - 1)) * 2;
}

void VgaTextMode::BufferWrite(char* buffer)
{
    /* Use Default Colors */
    BufferWrite(buffer, vga_color::WHITE, vga_color::BLACK);
}

void VgaTextMode::BufferWrite(char* buffer, vga_color FgColor, vga_color BgColor)
{
    for (int i = 0; buffer[i] != '\0'; i++) {
        /* Calculate Current Line */
        int CurrentLine = ((int)((CursorPos / 2) / VGATM_SCR_WIDTH));

        if (CurrentLine >= VGATM_SCR_HEIGHT)
            PageDown();

        if (buffer[i] == '\n') {
            /* Set Cursor Positions for Next Line */
            int NextLine = CurrentLine + 1;
            CursorPos = (NextLine * (VGATM_SCR_WIDTH * 2));

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