################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/twitter/twitter.c \
../protocols/twitter/twitter_ecmd.c 

OBJS += \
./protocols/twitter/twitter.o \
./protocols/twitter/twitter_ecmd.o 

C_DEPS += \
./protocols/twitter/twitter.d \
./protocols/twitter/twitter_ecmd.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/twitter/%.o: ../protocols/twitter/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


