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

#include <kernel/mem/pagemap.hpp>

using namespace tacOS::Kernel::VirtMem;

/// @brief Setup Structures, Configure Memory Paging
void PageMap::Intialize() {
    /*
        Paging in long mode consists of four maps - page directory pointer table, 
        page directory, page table, and the level-4 page map table (PML4). The
        structures are populated such that there exists a virtual address that
        could be mapped to a physical address. This routine currently performs
        identity paging.

        Refer:
        https://wiki.osdev.org/Paging
        https://wiki.osdev.org/Page_Tables
        https://os.phil-opp.com/page-tables/
        https://wiki.osdev.org/Setting_Up_Paging
    */
}