################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/artnet/artnet.c \
../protocols/artnet/artnet_net.c 

OBJS += \
./protocols/artnet/artnet.o \
./protocols/artnet/artnet_net.o 

C_DEPS += \
./protocols/artnet/artnet.d \
./protocols/artnet/artnet_net.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/artnet/%.o: ../protocols/artnet/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


