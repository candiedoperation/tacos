SECTIONS {
    /* Define a symbol to Indicate Kernel Start in Memory */
    OSLOADER_START = .;

    .boot :
    {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot)
    }

    .data :
    {
      *(.data)
    }

    .rodata :
    {
      *(.rodata)
    }

    .bss :
    {
      *(.bss)
    }

    .text :
    {
        *(.text)
    }

    /* Define a Symbol to indicate Kernel End in Memory */
    OSLOADER_END = .;
}