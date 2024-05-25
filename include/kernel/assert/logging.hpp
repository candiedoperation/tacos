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

#ifndef KERNEL_ASSERT_DEBUG_HPP
#define KERNEL_ASSERT_DEBUG_HPP

#include <kernel/types.hpp>

namespace tacOS {
namespace Kernel {
    class Logging {
    public:
        /// @brief Available Logging Levels
        enum struct LogLevel {
            DEBUG = 0,
            CRITICAL = 1,
            ERROR = 2,
            WARNING = 3,
            INFO = 4,
            NONE = 5
        };

        static LogLevel FilterLevel;
        static void LogMessage(LogLevel Level, char* Message);
    };
}
}

#endif