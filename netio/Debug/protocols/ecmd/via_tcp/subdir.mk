################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/ecmd/via_tcp/ecmd_net.c 

OBJS += \
./protocols/ecmd/via_tcp/ecmd_net.o 

C_DEPS += \
./protocols/ecmd/via_tcp/ecmd_net.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/ecmd/via_tcp/%.o: ../protocols/ecmd/via_tcp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


