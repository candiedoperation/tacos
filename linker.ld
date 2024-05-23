ENTRY(osload)

SECTIONS {
    /* The Current Location is set to 1MB */
    . = 1M;
    
    /* Define a symbol to Indicate Kernel Start in Memory */
    KRNL_START = .;

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
    KRNL_END = .;
}