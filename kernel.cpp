void printf(char* str) {
    unsigned short* vm = (unsigned short*) 0xb8000;
    for (int i = 0; str[i] != '\0'; i++) {
        vm[i] = ((unsigned short) 0x0f << 8) | str[i];
    }
}

void clear_screen() {
    unsigned short* vm = (unsigned short*) 0xb8000;
    for (int i = 0; i < 2000; i++) {
        vm[i] = ((unsigned short) 0x0f << 8) | 0x00;
    }
}

extern "C" void load_kernel() {
    /* Kernel Entrypoint */
    clear_screen();
    printf("Hello World! Code From C++ has been called from the assembly osloader.asm file!");

    while(1 == 1) {
        /* Prevent Exit */
    }
}