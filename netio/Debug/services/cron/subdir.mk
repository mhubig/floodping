################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../services/cron/cron.c \
../services/cron/cron_ecmd.c \
../services/cron/cron_static.c \
../services/cron/test.c 

OBJS += \
./services/cron/cron.o \
./services/cron/cron_ecmd.o \
./services/cron/cron_static.o \
./services/cron/test.o 

C_DEPS += \
./services/cron/cron.d \
./services/cron/cron_ecmd.d \
./services/cron/cron_static.d \
./services/cron/test.d 


# Each subdirectory must supply rules for building sources it contributes
services/cron/%.o: ../services/cron/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


