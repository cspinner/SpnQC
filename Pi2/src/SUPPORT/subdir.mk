################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SUPPORT/MadgwickAHRS.c 

OBJS += \
./src/SUPPORT/MadgwickAHRS.o 

C_DEPS += \
./src/SUPPORT/MadgwickAHRS.d 


# Each subdirectory must supply rules for building sources it contributes
src/SUPPORT/%.o: ../src/SUPPORT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-linux-gnueabi-gcc -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


