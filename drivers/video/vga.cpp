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
#include <asm/io.hpp>

using namespace tacOS::Drivers::Video;
using namespace tacOS::Kernel;
using namespace tacOS::ASM;

/* Initialize the Static VGA Buffer Addresses */
u8* VgaTextMode::MemoryAddress = (u8*)VGATM_MEM_ADDRESS;
u16 VgaTextMode::CursorPos = 0;

/// @brief Handles rudimentary non-buffered page down scroll
void VgaTextMode::PageDown()
{
    /* Scroll Up, Copy lines up and make space at bottom. */
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
    BufferWrite(buffer, Color::WHITE, Color::BLACK);
}

void VgaTextMode::BufferWrite(char* buffer, Color FgColor, Color BgColor)
{
    for (int i = 0; buffer[i] != '\0'; i++) {
        /* Calculate Current Line and Text Properties */
        u8 TextProperties = ((u8)BgColor << 4) | ((u8)FgColor);
        int CurrentLine = ((int)((CursorPos / 2) / VGATM_SCR_WIDTH));

        if (CurrentLine >= VGATM_SCR_HEIGHT)
            PageDown();

        /* 0x0A (Line Feed) is \n */
        if (buffer[i] == '\n') {
            /* Set Cursor Positions for Next Line */
            int NextLine = CurrentLine + 1;
            CursorPos = (NextLine * (VGATM_SCR_WIDTH * 2));

            /* Don't Print \n */
            continue;
        }

        /* 0x08 (Backspace ASCII) */
        if (buffer[i] == 0x08) {
            MemoryAddress[CursorPos - 2] = 0; /* Previous TextInfo Cursor */
            MemoryAddress[CursorPos - 1] = TextProperties; /* Previous TextAttr Cursor */
            CursorPos -= 2;
            continue;
        }

        /* Update Memory Location */
        MemoryAddress[CursorPos] = buffer[i];
        MemoryAddress[CursorPos + 1] = TextProperties;

        /* Move Memory Position */
        CursorPos += 2;
    }

    /* Update the VGA Text Mode Cursor */
    // Temporary Impl. Write Drivers. RN, grub enables cursor. fix that.
    
    /* Calculate Current Line */
    u16 pos = CursorPos / 2;

    IO::outb(0x3D4, 0x0F);
    IO::outb(0x3D5, (u8) (pos & 0xFF));

    IO::outb(0x3D4, 0x0E);
	IO::outb(0x3D5, (u8) ((pos >> 8) & 0xFF));
}