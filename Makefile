#Global Variables
GPP_PARAMETERS = -m32
AS_PARAMETERS = --32
LD_PARAMETERS = -melf_i386

#Other Variables
KRNL_DEPENDENCIES = osloader.o kernel.o

#Compile *.cpp to *.o
%.o: %.cpp
	g++ $(GPP_PARAMETERS) -o build/$@ -c $<

#Compile *.s to *.o
%.o: %.s
	as $(AS_PARAMETERS) -o build/$@ -c $<

#Kernel.bin depends on linker.ld, defined files
kernel.bin: linker.ld $(KRNL_DEPENDENCIES)
	ld $(LD_PARAMETERS) -T $< -o build/$@ $(KRNL_DEPENDENCIES)