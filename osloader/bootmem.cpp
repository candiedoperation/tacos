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

#include <osloader/bootmem.hpp>

/* Define Statics */
extern VMMPageTable osloader_pml4t;
alignas(4096) VMMPageTable OSLoaderPML4Tbl;
VMMPageTable OSLoaderPDPTbl;
VMMPageTable OSLoaderPDTbl;
VMMPageTable OSLoaderPTbl;

extern "C" void bootmem_init() {   
    for (;;) {} 
    /* Identity map first 2MB */
    for (int i = 0; i < OSLOADER_VMM_PTBL_MAXENTRIES; i++) {
        OSLoaderPTbl.Entries[i] = (i * OSLOADER_VMM_BLOCKSIZE) | 3;
        //osloader_pml4t.Entries[i] = 0;
    }

    OSLoaderPDTbl.Entries[0] = ((u64) &OSLoaderPTbl) | 3;
    OSLoaderPDPTbl.Entries[0] = ((u64) &OSLoaderPDTbl) | 3;
    OSLoaderPML4Tbl.Entries[0] = ((u64) &OSLoaderPDPTbl) | 3;

    //__asm__ volatile ("mov %0, %%eax" : : "r" (&osloader_pml4t) : "memory");
    for(;;) {
        
    }

    //__asm__ volatile ("mov %0, %%cr3" : : "r" (0x00) : "memory");
}