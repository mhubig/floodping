################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/mysql/mysql.c \
../protocols/mysql/mysql_ecmd.c 

OBJS += \
./protocols/mysql/mysql.o \
./protocols/mysql/mysql_ecmd.o 

C_DEPS += \
./protocols/mysql/mysql.d \
./protocols/mysql/mysql_ecmd.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/mysql/%.o: ../protocols/mysql/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


