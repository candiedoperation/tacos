# osloader.asm

`.global start` defines the global function **start**

`section .multiboot`

- 📖 The Multiboot header is defined at Section 3.1.1 (The Layout of a Multiboot Header) of the [Multiboot Specification](https://nongnu.askapache.com/grub/phcoder/multiboot.pdf) document

- Makes code Mutliboot compliant for bootloaders, like Grub2, to run the instruction

- `mboot_start` through `mboot_end` define the data block for Multiboot Compliance:
  
  - `dd` - Define Double, defines 32bit Constant
  
  - `dw` - Define Word, defines 16bit Constant
  
  - To be recogonized as a Bootable file, the Multiboot specification requires a Magic Number. It's defined as `dd 0xe85250d6`.
  
  - `dd 0` sets the architecture to `protected i386` mode
  
  - `dd mboot_end - mboot_start` adds the size of the whole Multiboot header section
  
  - The Checksum field is `-(magic + architecture + header_length)` 
  
  - Checksum is being subtracted from `0x100000000` ($ = 2^{32}$) to prevent the 32bit variable from overflowing.
  
  - The last block represents a tag of type `END`. It's necessary for multiboot to work. The data structure for a tag is specified in Section 3.1.3 (General tag Structure) of the [Multiboot Specification](https://nongnu.askapache.com/grub/phcoder/multiboot.pdf) document.

`.section .text`

- Defines Main Executable Code

- `bits 32` sets CPU to the 32-bit Protected Mode instruction set

- `start` defines the entrypoint for the kernel
