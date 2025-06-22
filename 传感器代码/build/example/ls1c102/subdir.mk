#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../example/ls1c102/1c102_cmd.c \
../example/ls1c102/1c102_cmdline.c

OBJS += \
./example/ls1c102/1c102_cmd.o \
./example/ls1c102/1c102_cmdline.o

C_DEPS += \
./example/ls1c102/1c102_cmd.d \
./example/ls1c102/1c102_cmdline.d

# Each subdirectory must supply rules for building sources it contributes
example/ls1c102/%.o: ../example/ls1c102/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1C102 -DBSP_USE_RTC -std=gnu99 -ffunction-sections -fdata-sections -msoft-float -fsched-pressure  -O2 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../drivers/include" -I"../drivers/private/ls1c102" -I"../drivers/public" -I"../system" -I"../src/rtc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

