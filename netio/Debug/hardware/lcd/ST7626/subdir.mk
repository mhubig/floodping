################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/lcd/ST7626/ST7626.c 

OBJS += \
./hardware/lcd/ST7626/ST7626.o 

C_DEPS += \
./hardware/lcd/ST7626/ST7626.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/lcd/ST7626/%.o: ../hardware/lcd/ST7626/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


