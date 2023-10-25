ENTRY(osload)

SECTIONS {
    . = 1M;

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
}