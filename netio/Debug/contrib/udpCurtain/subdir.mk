################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../contrib/udpCurtain/curtain_udp_test.c 

OBJS += \
./contrib/udpCurtain/curtain_udp_test.o 

C_DEPS += \
./contrib/udpCurtain/curtain_udp_test.d 


# Each subdirectory must supply rules for building sources it contributes
contrib/udpCurtain/%.o: ../contrib/udpCurtain/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


