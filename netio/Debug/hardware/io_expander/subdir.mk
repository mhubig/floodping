################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/io_expander/hc165.c \
../hardware/io_expander/hc595.c 

OBJS += \
./hardware/io_expander/hc165.o \
./hardware/io_expander/hc595.o 

C_DEPS += \
./hardware/io_expander/hc165.d \
./hardware/io_expander/hc595.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/io_expander/%.o: ../hardware/io_expander/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


