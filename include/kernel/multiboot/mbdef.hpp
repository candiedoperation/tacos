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

#ifndef KERNEL_MBOOTDEF_HPP
#define KERNEL_MBOOTDEF_HPP

#include <kernel/types.hpp>

using namespace tacOS::Kernel;

namespace tacOS {
namespace Kernel {
    /// @brief Contains Structure Defenitions for Multiboot 2
    class MBootDef {
    public:
        struct MultibootTag {
            u32 TagType;
            u32 Size;
        };

        struct MultibootInfo {
            u32 Length;
            u32 Reserved;
            MultibootTag Tags[0];
        };
    };
}
}

#endif