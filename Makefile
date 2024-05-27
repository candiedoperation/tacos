#Global Variables
GPP_PARAMETERS = -m64 -I include -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
AS_PARAMETERS = --32
LD_PARAMETERS = -n

#Other Variables  $(BUILD_PATH)/kernel.o
BUILD_PATH = build
KRNL_DEPENDENCIES = $(BUILD_PATH)/osloader/osloader.o \
					$(BUILD_PATH)/osloader/os64loader.o \
					$(BUILD_PATH)/tools/kernelrtl/printf.o \
					$(BUILD_PATH)/tools/kernelrtl/strings.o \
					$(BUILD_PATH)/drivers/hal/pic8259.o \
					$(BUILD_PATH)/drivers/hal/apic.o \
					$(BUILD_PATH)/drivers/hal/virtkbd.o \
					$(BUILD_PATH)/drivers/ps2/keyboard.o \
					$(BUILD_PATH)/drivers/acpi/acpidef.o \
					$(BUILD_PATH)/drivers/acpi/acpipvdr.o \
					$(BUILD_PATH)/drivers/video/vga.o \
					$(BUILD_PATH)/kernel/assert/logging.o \
					$(BUILD_PATH)/kernel/mem/bootmem.o \
					$(BUILD_PATH)/kernel/mem/physicalmm.o \
					$(BUILD_PATH)/kernel/mem/virtualmm.o \
					$(BUILD_PATH)/kernel/multiboot/mbpvdr.o \
					$(BUILD_PATH)/kernel/interrupts/isrdef.o \
					$(BUILD_PATH)/kernel/interrupts/intrdef.o \
					$(BUILD_PATH)/kernel/tacoskrnl.o

#Define PHONY
.PHONY = run run-debug clean

#Compile *.cpp to *.o
$(BUILD_PATH)/%.o: %.cpp
	mkdir -p $(dir $@)
	g++ $(GPP_PARAMETERS) -o $@ -c $<

#Compile *.asm to *.o
$(BUILD_PATH)/%.o: %.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

#Kernel.bin depends on linker.ld, defined files
$(BUILD_PATH)/kernel.bin: linker.ld $(KRNL_DEPENDENCIES)
	mkdir -p $(dir $@)
	ld $(LD_PARAMETERS) -T $< -o $@ $(KRNL_DEPENDENCIES)

$(BUILD_PATH)/tacos.iso: $(BUILD_PATH)/kernel.bin
# Copy kernel.bin, Create GRUB Config, Folder needs to be named "boot"
	mkdir -p $(BUILD_PATH)/iso_build/boot/grub

	cp $< $(BUILD_PATH)/iso_build/boot
	echo 'set timeout=5' >> $(BUILD_PATH)/iso_build/boot/grub/grub.cfg
	echo '' >> $(BUILD_PATH)/iso_build/boot/grub.cfg
	echo 'menuentry "tacOS 0.1" {' >> $(BUILD_PATH)/iso_build/boot/grub/grub.cfg
	echo '	multiboot2 /boot/kernel.bin' >> $(BUILD_PATH)/iso_build/boot/grub/grub.cfg
	echo '	boot' >> $(BUILD_PATH)/iso_build/boot/grub/grub.cfg
	echo '}' >> $(BUILD_PATH)/iso_build/boot/grub/grub.cfg

# Create ISO image
	grub-mkrescue --output=$@ $(BUILD_PATH)/iso_build
	rm -rf $(BUILD_PATH)/iso_build/

run: $(BUILD_PATH)/tacos.iso
	qemu-system-x86_64 -cdrom $(BUILD_PATH)/tacos.iso

run-uefi: $(BUILD_PATH)/tacos.iso
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -cdrom $(BUILD_PATH)/tacos.iso

run-vbox: $(BUILD_PATH)/tacos.iso
	virtualboxvm --startvm tacOS --dbg

run-debug: $(BUILD_PATH)/tacos.iso
	qemu-system-x86_64  -s -S -cdrom $(BUILD_PATH)/tacos.iso
	echo "-d int -no-reboot -no-shutdown"

clean:
	rm -rf $(BUILD_PATH)/