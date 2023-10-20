# tacOS Documentation

In this documentation folder, every file except the Readme refers to a particular code file in the source. The directory structure and file names of source files are exactly matched in the documentation to aid easy access of documentation for a specific source file.

# Required Build Tools

⚠️ Package names have been listed to match Debian distributions and the `apt` package manager. Your distro might have different package names.

- The `build-essentials` package. Tools include `g++`, `ld`, `make`, etc.

- The `nasm` assembler

- `xorriso` and `grub-pc-bin` to build QEMU compatible images

- `grub-mkrescue` to build the `.iso` disk images

- `qemu-system-x86_64` to enable `make run`

# Building from Source

There are two ways of getting the source code compiled, up and running. Both of these methods have been listed below:

## 1. Building the ISO

- Install the Required Build tools

- `make build/tacos.iso`

- Run the ISO using a Virtual Machine or bare metal

## 2. Building and Running the ISO

- Install the Required Build tools

- `make run`
