################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/mdns_sd/mdns_sd.c \
../protocols/mdns_sd/mdns_sd_net.c 

OBJS += \
./protocols/mdns_sd/mdns_sd.o \
./protocols/mdns_sd/mdns_sd_net.o 

C_DEPS += \
./protocols/mdns_sd/mdns_sd.d \
./protocols/mdns_sd/mdns_sd_net.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/mdns_sd/%.o: ../protocols/mdns_sd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


