#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../user/ls1c102/1c102_main.c \
../user/ls1c102/1c102_Interrupt.c

OBJS += \
./user/ls1c102/1c102_main.o \
./user/ls1c102/1c102_Interrupt.o

C_DEPS += \
./user/ls1c102/1c102_main.d \
./user/ls1c102/1c102_Interrupt.d

# Each subdirectory must supply rules for building sources it contributes
user/ls1c102/%.o: ../user/ls1c102/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1C102 -DBSP_USE_RTC -std=gnu99 -ffunction-sections -fdata-sections -msoft-float -fsched-pressure  -O2 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../drivers/include" -I"../drivers/private/ls1c102" -I"../drivers/public" -I"../system" -I"../src/rtc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

