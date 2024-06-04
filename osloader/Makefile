# OSLoader Bootstrap Makefile
BUILD_PATH = ../build/osloader
GPP32_PARAMETERS = -m32 -I ../include -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
LD_PARAMETERS = -m elf_i386 -i # -i for Incremental Linking

TARGETS = os32loader.o \
		  bootmem.o \
		  os64loader.o \

all: init $(TARGETS)
	cd $(BUILD_PATH) && \
	ld $(LD_PARAMETERS) -T ../../osloader/linker.ld $(TARGETS) -o osloader.o && \
	rm $(TARGETS)

init:
	mkdir -p $(BUILD_PATH)

# Compile *.cpp to *.o
%.o: %.cpp
	g++ $(GPP32_PARAMETERS) -o $(BUILD_PATH)/$@ -c $<
#	objcopy -I elf32-x86-64 -O elf64-x86-64 $(BUILD_PATH)/$@

# Compile *.asm to *.o
%.o: %.asm
	nasm -f elf32 $< -o $(BUILD_PATH)/$@