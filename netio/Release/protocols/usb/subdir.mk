################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/usb/ecmd_usb.c \
../protocols/usb/usb.c \
../protocols/usb/usb_hid_keyboard.c \
../protocols/usb/usb_hid_mouse.c \
../protocols/usb/usb_net.c 

OBJS += \
./protocols/usb/ecmd_usb.o \
./protocols/usb/usb.o \
./protocols/usb/usb_hid_keyboard.o \
./protocols/usb/usb_hid_mouse.o \
./protocols/usb/usb_net.o 

C_DEPS += \
./protocols/usb/ecmd_usb.d \
./protocols/usb/usb.d \
./protocols/usb/usb_hid_keyboard.d \
./protocols/usb/usb_hid_mouse.d \
./protocols/usb/usb_net.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/usb/%.o: ../protocols/usb/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


