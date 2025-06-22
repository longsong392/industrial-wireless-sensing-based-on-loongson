#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../drivers/private/ls1c102/ls1c102_ptimer.c \
../drivers/private/ls1c102/ls1c102_touch.c \
../drivers/private/ls1c102/ls1c102_vpwm.c \
../drivers/private/ls1c102/ls1c102_adc.c \
../drivers/private/ls1c102/ls1c102_i2c.c

OBJS += \
./drivers/private/ls1c102/ls1c102_ptimer.o \
./drivers/private/ls1c102/ls1c102_touch.o \
./drivers/private/ls1c102/ls1c102_vpwm.o \
./drivers/private/ls1c102/ls1c102_adc.o \
./drivers/private/ls1c102/ls1c102_i2c.o

C_DEPS += \
./drivers/private/ls1c102/ls1c102_ptimer.d \
./drivers/private/ls1c102/ls1c102_touch.d \
./drivers/private/ls1c102/ls1c102_vpwm.d \
./drivers/private/ls1c102/ls1c102_adc.d \
./drivers/private/ls1c102/ls1c102_i2c.d

# Each subdirectory must supply rules for building sources it contributes
drivers/private/ls1c102/%.o: ../drivers/private/ls1c102/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1C102 -DBSP_USE_RTC -std=gnu99 -ffunction-sections -fdata-sections -msoft-float -fsched-pressure  -O2 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../drivers/include" -I"../drivers/private/ls1c102" -I"../drivers/public" -I"../system" -I"../src/rtc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

