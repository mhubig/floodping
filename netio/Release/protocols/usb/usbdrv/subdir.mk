################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/usb/usbdrv/oddebug.c \
../protocols/usb/usbdrv/usbdrv.c 

S_UPPER_SRCS += \
../protocols/usb/usbdrv/usbdrvasm.S 

ASM_SRCS += \
../protocols/usb/usbdrv/usbdrvasm.asm 

OBJS += \
./protocols/usb/usbdrv/oddebug.o \
./protocols/usb/usbdrv/usbdrv.o \
./protocols/usb/usbdrv/usbdrvasm.o 

C_DEPS += \
./protocols/usb/usbdrv/oddebug.d \
./protocols/usb/usbdrv/usbdrv.d 

ASM_DEPS += \
./protocols/usb/usbdrv/usbdrvasm.d 

S_UPPER_DEPS += \
./protocols/usb/usbdrv/usbdrvasm.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/usb/usbdrv/%.o: ../protocols/usb/usbdrv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

protocols/usb/usbdrv/%.o: ../protocols/usb/usbdrv/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega644p -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

protocols/usb/usbdrv/%.o: ../protocols/usb/usbdrv/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega644p -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


