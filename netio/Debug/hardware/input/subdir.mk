################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/input/game_commons.c \
../hardware/input/hr20.c 

OBJS += \
./hardware/input/game_commons.o \
./hardware/input/hr20.o 

C_DEPS += \
./hardware/input/game_commons.d \
./hardware/input/hr20.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/input/%.o: ../hardware/input/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


