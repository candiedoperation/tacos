© Copyright 2023 Atheesh Thirumalairajan

View LICENSE.md for More Information

---

# Makefile

`GPP_PARAMETERS` let's us set the compiler parameters for `g++` . The `-m32` flag compiles the code for 32-bit processors.

`AS_PARAMETERS` defines parameters for the GNU Assembler. The `--32` flag assembles the code for 32-bit processors.

`LD_PARAMETERS` defines parameters for the `ld` linker. The `-melf_i386` flag sets linker for 32-bit processors.

`KRNL_DEPENDENCIES` defines files the Linker should take in as input for appropriate kernel compilation

`linker.ld` defines instructions for the Linker.