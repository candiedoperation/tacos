void printf(char* str) {
    unsigned short* vm = (unsigned short*) 0xb8000;
    for (int i = 0; str[i] != '\0'; ++i) {
        vm[i] = (vm[i] & 0xFF00) | str[i];
    }
}

extern "C" void InitKernel() {
    /* Kernel Entrypoint */
    printf("HEllo!");

    while(1 == 1) {
        /* Prevent Exit */
    }
}