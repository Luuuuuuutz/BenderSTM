################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/micro_wait.s 

C_SRCS += \
../src/fat16.c \
../src/ff.c \
../src/gcode.c \
../src/main.c \
../src/motorSubsystem.c \
../src/sd.c \
../src/sdmm.c \
../src/spi.c \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/usart.c 

OBJS += \
./src/fat16.o \
./src/ff.o \
./src/gcode.o \
./src/main.o \
./src/micro_wait.o \
./src/motorSubsystem.o \
./src/sd.o \
./src/sdmm.o \
./src/spi.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/usart.o 

C_DEPS += \
./src/fat16.d \
./src/ff.d \
./src/gcode.d \
./src/main.d \
./src/motorSubsystem.d \
./src/sd.d \
./src/sdmm.d \
./src/spi.d \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"C:/Users/eric/workspace/BenderSTM/Utilities" -I"C:/Users/eric/workspace/BenderSTM/StdPeriph_Driver/inc" -I"C:/Users/eric/workspace/BenderSTM/inc" -I"C:/Users/eric/workspace/BenderSTM/CMSIS/device" -I"C:/Users/eric/workspace/BenderSTM/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"C:/Users/eric/workspace/BenderSTM/Utilities" -I"C:/Users/eric/workspace/BenderSTM/StdPeriph_Driver/inc" -I"C:/Users/eric/workspace/BenderSTM/inc" -I"C:/Users/eric/workspace/BenderSTM/CMSIS/device" -I"C:/Users/eric/workspace/BenderSTM/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


