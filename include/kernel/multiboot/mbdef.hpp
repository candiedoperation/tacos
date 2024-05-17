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
        /// @brief Enumeration of Multiboot2 Tag Types
        enum Tag {
            BOOT_CMD_LINE = 1,
            BTLDR_NAME = 2,
            MODULES = 3,
            MEMORY_INFO = 4,
            BIOS_BOOT_DEVICE = 5,
            MEMORY_MAP = 6,
            VBE_INFO = 7,
            FRAMEBUFFER_INFO = 8,
            ELF_SYMBOLS = 9,
            APM_INFO = 10,
            EFI_32_SYSTABLE = 11,
            EFI_64_SYSTABLE = 12,
            SMBIOS_TABLE = 13,
            ACPI_RSDP_V1 = 14,
            ACPI_RSDP_V2 = 15,
            NETWORK_INFO = 16,
            EFI_MEMORY_MAP = 17,
            EFI_BOOTSRV_TERMINATED = 18,
            EFI_32_IMAGE = 19,
            EFI_64_IMAGE = 20,
            IMG_PHY_ADDR = 21
        };

        struct TagHeader {
            u32 TagType;
            u32 Size;
        };

        struct MultibootInfo {
            u32 Length;
            u32 Reserved;
            TagHeader Tags[0];
        };

        /// @brief Basic memory information Structure
        struct MemoryInfo
        {
            TagHeader Header;
            u32 MemLower;
            u32 MemUpper;
        };

        /// @brief BIOS Boot device Structure
        struct BIOSBootDevice
        {
            TagHeader Header;
            u32 BiosDevice;
            u32 Partition;
            u32 SubPartition;
        };

        /// @brief Boot command line Structure
        struct BootCMDLine
        {
            TagHeader Header;
            char* CmdLine;
        };

        /// @brief Modules Structure
        struct Modules
        {
            TagHeader Header;
            u32 ModuleStart;
            u32 ModuleEnd;
            char* InfoStr;
        };

        /// @brief ELF Symbols Structure
        struct ELFSymbols
        {
            TagHeader Header;
            u16 Num;
            u16 EntrySize;
            u16 Shndx;
            u16 Reserved;
            /* Variable Length Section Headers. Use Ptr + 1 to access */
        };

        /// @brief Memory Map Structure
        struct MemoryMap
        {
            TagHeader Header;
            u32 EntrySize;
            u32 EntryVersion;
            /* Variable Length Entries. Use Ptr + 1 to access */
        };

        /// @brief Memory Map Entry Structure
        struct MemoryMapEntry
        {
            u64 BaseAddress;
            u64 Length;
            u32 Type;
            u32 Reserved;
        };

        /// @brief Boot Loader Name Structure
        struct BtldrName
        {
            TagHeader Header;
            char* Name;
        };

        /// @brief APM Table Structure
        struct APMTable
        {
            TagHeader Header;
            u16 Version;
            u16 CSeg;
            u32 Offset;
            u16 CSeg16;
            u16 DSeg;
            u16 Flags;
            u16 CSegLength;
            u16 CSeg16Length;
            u16 DSegLength;
        };

        /// @brief VBE Info Structure
        struct VBEInfo
        {
            TagHeader Header;
            u16 Mode;
            u16 InterfaceSeg;
            u16 InterfaceOff;
            u16 InterfaceLength;
            u8 ControlInfo[512];
            u8 ModeInfo[256];
        };

        /// @brief Framebuffer Info Structure
        struct FramebufferInfo
        {
            TagHeader Header;
            u64 Address;
            u32 Pitch;
            u32 Width;
            u32 Height;
            u8 BitsPerPixel;
            u8 Type;
            u8 Reserved;
            /* Variable Length Color Info. Use Ptr + 1 to access. */
        };

        /// @brief Framebuffer Color Info Structure
        struct FramebufferColorInfo
        {
            u32 PaletteNumColors;
            /* Variable Length Palette. Use Ptr + 1 to access. */
        };

        /// @brief Framebuffer Color Info Palette Structure
        struct FramebufferColorInfoPalette
        {
            u8 RedValue;
            u8 GreenValue;
            u8 BlueValue;
        };

        /// @brief Framebuffer Info Color Type Structure
        struct FrambufferInfoColorType
        {
            u8 RedFieldPosition;
            u8 RedMaskSize;
            u8 GreenFieldPosition;
            u8 GreenMaskSize;
            u8 BlueFieldPosition;
            u8 BlueMaskSize;
        };

        /// @brief EFI 32-bit System Table Pointer Structure
        struct EFI32SysTablePointer
        {
            TagHeader Header;
            u32 Pointer;
        };

        /// @brief EFI 64-bit System Table Pointer Structure
        struct EFI64SysTablePointer
        {
            TagHeader Header;
            u32 Pointer;
        };

        /// @brief SMBIOS Tables Structure
        struct SMBIOSTables
        {
            TagHeader Header;
            u8 Major;
            u8 Minor;
            u8 Reserved[6];
        };

        /// @brief ACPI RSDPv1 Structure
        struct AcpiRsdpV1
        {
            TagHeader Header;
            /* COPY OF RSDP? */
        };

        /// @brief ACPI RSDPv2 Structure
        struct AcpiRsdpV2
        {
            TagHeader Header;
            /* COPY OF RSDP? */
        };

        /// @brief Networking Information Structure
        struct NetworkInfo
        {
            TagHeader Header;
            /* DHCP ACK Copy? */
        };

        /// @brief EFI Memory Map Structure
        struct EFIMemoryMap
        {
            TagHeader Header;
            u32 DescriptorSize;
            u32 DescriptorVersion;
        };

        /// @brief EFI Boot Services Not Terminated Structure
        struct EFIBootsrvNotTerminated
        {
            TagHeader Header;
            /* Presence of Tag indicated ExitBootServices wasn't called */
        };

        /// @brief EFI 32-bit Image Handle Pointer Structure
        struct EFI32ImageHandle
        {
            TagHeader Header;
            u32 Pointer;
        };

        /// @brief EFI 64-bit Image Handle Pointer Structure
        struct EFI64ImageHandle
        {
            TagHeader Header;
            u64 Pointer;
        };

        /// @brief Image Load Base Physical Address Structure
        struct ImgLoadPhysicalAddr
        {
            TagHeader Header;
            u32 BaseAddress;
        };
    };
}
}

#endif