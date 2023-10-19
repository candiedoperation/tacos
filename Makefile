#Global Variables
GPP_PARAMETERS = -m32
AS_PARAMETERS = --32
LD_PARAMETERS = -melf_i386

#Other Variables
KRNL_DEPENDENCIES = osloader.o kernel.o
BUILD_PATH = build

#Compile *.cpp to *.o
%.o: %.cpp
	mkdir -p $(BUILD_PATH)
	g++ $(GPP_PARAMETERS) -o $(BUILD_PATH)/$@ -c $<

#Compile *.asm to *.o
%.o: %.asm
	mkdir -p $(BUILD_PATH)
	as $(AS_PARAMETERS) -o $(BUILD_PATH)/$@ -c $<

#Kernel.bin depends on linker.ld, defined files
kernel.bin: linker.ld $(KRNL_DEPENDENCIES)
	mkdir -p $(BUILD_PATH)
	ld $(LD_PARAMETERS) -T $< -o $(BUILD_PATH)/$@ $(KRNL_DEPENDENCIES)

clean:
	rm -rf $(BUILD_PATH)/