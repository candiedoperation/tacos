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

#ifndef OSLOADER_BOOTMEM32_HPP
#define OSLOADER_BOOTMEM32_HPP

#define OSLOADER_VMM_PTBL_MAXENTRIES 512
#define OSLOADER_VMM_BLOCKSIZE 4096

typedef unsigned long long u64;
typedef u64 VMMPTEntry;

struct VMMPageTable
{
    VMMPTEntry Entries[OSLOADER_VMM_PTBL_MAXENTRIES];   
};


extern "C" void bootmem_init();

#endif