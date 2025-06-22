#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../drivers/public/ls1x_clock.c \
../drivers/public/ls1x_common.c \
../drivers/public/ls1x_exti.c \
../drivers/public/ls1x_flash.c \
../drivers/public/ls1x_gpio.c \
../drivers/public/ls1x_latimer.c \
../drivers/public/ls1x_logo.c \
../drivers/public/ls1x_pmu.c \
../drivers/public/ls1x_printf.c \
../drivers/public/ls1x_rtc.c \
../drivers/public/ls1x_string.c \
../drivers/public/ls1x_uart.c \
../drivers/public/ls1x_wdg.c \
../drivers/public/ls1x_spi.c

OBJS += \
./drivers/public/ls1x_clock.o \
./drivers/public/ls1x_common.o \
./drivers/public/ls1x_exti.o \
./drivers/public/ls1x_flash.o \
./drivers/public/ls1x_gpio.o \
./drivers/public/ls1x_latimer.o \
./drivers/public/ls1x_logo.o \
./drivers/public/ls1x_pmu.o \
./drivers/public/ls1x_printf.o \
./drivers/public/ls1x_rtc.o \
./drivers/public/ls1x_string.o \
./drivers/public/ls1x_uart.o \
./drivers/public/ls1x_wdg.o \
./drivers/public/ls1x_spi.o

C_DEPS += \
./drivers/public/ls1x_clock.d \
./drivers/public/ls1x_common.d \
./drivers/public/ls1x_exti.d \
./drivers/public/ls1x_flash.d \
./drivers/public/ls1x_gpio.d \
./drivers/public/ls1x_latimer.d \
./drivers/public/ls1x_logo.d \
./drivers/public/ls1x_pmu.d \
./drivers/public/ls1x_printf.d \
./drivers/public/ls1x_rtc.d \
./drivers/public/ls1x_string.d \
./drivers/public/ls1x_uart.d \
./drivers/public/ls1x_wdg.d \
./drivers/public/ls1x_spi.d

# Each subdirectory must supply rules for building sources it contributes
drivers/public/%.o: ../drivers/public/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1C102 -DBSP_USE_RTC -std=gnu99 -ffunction-sections -fdata-sections -msoft-float -fsched-pressure  -O2 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../drivers/include" -I"../drivers/private/ls1c102" -I"../drivers/public" -I"../system" -I"../src/rtc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

