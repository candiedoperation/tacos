#Define GRUB Specifics
.set GRUB_MAGIC, 0x1badb002
.set GRUB_FLAGS, (1<<0 | 1<<1)
.set GRUB_CHKSUM, -(GRUB_MAGIC + GRUB_FLAGS)

#Define the GRUB Multiboot section
.section .multiboot
    .long GRUB_MAGIC
    .long GRUB_FLAGS
    .long GRUB_CHKSUM

.section .text
.extern InitKernel
.global entry

entry:
    mov $KernelStack, %esp
    call InitKernel

.section .bss

#Add 1MB Padding
.space 1024 * 1024

KernelStack:
