################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/i2c/slave/i2c_slave.c \
../hardware/i2c/slave/i2c_slave_net.c 

OBJS += \
./hardware/i2c/slave/i2c_slave.o \
./hardware/i2c/slave/i2c_slave_net.o 

C_DEPS += \
./hardware/i2c/slave/i2c_slave.d \
./hardware/i2c/slave/i2c_slave_net.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/i2c/slave/%.o: ../hardware/i2c/slave/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


