################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/APP/SUPPORT/MadgwickAHRS.c 

OBJS += \
./src/APP/SUPPORT/MadgwickAHRS.o 

C_DEPS += \
./src/APP/SUPPORT/MadgwickAHRS.d 


# Each subdirectory must supply rules for building sources it contributes
src/APP/SUPPORT/%.o: ../src/APP/SUPPORT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-linux-gnueabi-gcc -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -I../SpnQC/pigpio -I../SpnQC/SpnQC/src/HAL -I../SpnQC/wiringPi/include -I../SpnQC/SpnQC/src/SUPPORT -I../SpnQC/SpnQC/src/SENSE -I../SpnQC/SpnQC/src/CONTROL -I../SpnQC/SpnQC/src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


