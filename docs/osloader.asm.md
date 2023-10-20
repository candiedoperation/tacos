# osloader.asm

`.section .text`

- Defines Main Executable Code

- `.extern InitKernel` defines the external C++ function

- `.global entry` defines the global function **entry**

`.section .bss`

- Defines Uninitialized Variables

- Adds **1 MB** padding to fit the kernel in place

- 
