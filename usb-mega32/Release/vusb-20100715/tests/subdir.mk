################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vusb-20100715/tests/main.c \
../vusb-20100715/tests/null.c 

OBJS += \
./vusb-20100715/tests/main.o \
./vusb-20100715/tests/null.o 

C_DEPS += \
./vusb-20100715/tests/main.d \
./vusb-20100715/tests/null.d 


# Each subdirectory must supply rules for building sources it contributes
vusb-20100715/tests/%.o: ../vusb-20100715/tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=12000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

