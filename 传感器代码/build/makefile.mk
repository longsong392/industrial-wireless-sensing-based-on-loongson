#
# Auto-generated file. Do not edit!
#

GCC_SPECS := D:/LoongIDE/la32-tool/loongarch32-newlib-elf/ls1c102
OS := bare

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include objects.mk
-include drivers/private/ls1c102/subdir.mk
-include drivers/public/subdir.mk
-include example/ls1c102/subdir.mk
-include example/src/rtc/subdir.mk
-include system/subdir.mk
-include user/subdir.mk
-include user/ls1c102/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
endif

-include ../makefile.defs

OUT_ELF=rtc_demo_.exe
OUT_MAP=rtc_demo_.map
OUT_BIN=rtc_demo_.bin

LINKCMDS=../ld.script

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: $(OUT_ELF)

# Tool invocations
$(OUT_ELF): $(STARTO) $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: LoongArch32 ELF C Linker'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe  -mabi=ilp32s -march=loongarch32 -G0 -nostartfiles -nodefaultlibs -nostdlib -Wl,-gc-sections -static -T $(LINKCMDS) -o $(OUT_ELF) $(STARTO) $(OBJS) $(USER_OBJS) $(LIBS) 
	@echo 'Invoking: LoongArch32 ELF C Objcopy'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-objcopy.exe -O binary $(OUT_ELF) $(OUT_BIN)
	@echo 'Invoking: LoongArch32 ELF C Size'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-size.exe $(OUT_ELF)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(STARTO) $(OBJS) $(C_DEPS) $(CPP_DEPS) $(ASM_DEPS) $(EXECUTABLES) $(OUT_ELF) $(OUT_MAP) $(OUT_BIN)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets


