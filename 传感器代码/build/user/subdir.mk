#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../user/gpio.c \
../user/timer.c \
../user/BH1750.c \
../user/AHT20.c \
../user/myiic.c \
../user/BMP280.c \
../user/iic_bus.c \
../user/A141A.c \
../user/A158A.c \
../user/MQ2.c \
../user/DHT11.c \
../user/LORA.c \
../user/SHT30.c \
../user/SM4.c

OBJS += \
./user/gpio.o \
./user/timer.o \
./user/BH1750.o \
./user/AHT20.o \
./user/myiic.o \
./user/BMP280.o \
./user/iic_bus.o \
./user/A141A.o \
./user/A158A.o \
./user/MQ2.o \
./user/DHT11.o \
./user/LORA.o \
./user/SHT30.o \
./user/SM4.o

C_DEPS += \
./user/gpio.d \
./user/timer.d \
./user/BH1750.d \
./user/AHT20.d \
./user/myiic.d \
./user/BMP280.d \
./user/iic_bus.d \
./user/A141A.d \
./user/A158A.d \
./user/MQ2.d \
./user/DHT11.d \
./user/LORA.d \
./user/SHT30.d \
./user/SM4.d

# Each subdirectory must supply rules for building sources it contributes
user/%.o: ../user/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: LoongArch32 ELF C Compiler'
	D:/LoongIDE/la32-tool/bin/loongarch32-newlib-elf-gcc.exe -mabi=ilp32s -march=loongarch32 -G0 -DLS1C102 -DBSP_USE_RTC -std=gnu99 -ffunction-sections -fdata-sections -msoft-float -fsched-pressure  -O2 -fno-builtin -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../include" -I"../drivers/include" -I"../drivers/private/ls1c102" -I"../drivers/public" -I"../system" -I"../src/rtc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

