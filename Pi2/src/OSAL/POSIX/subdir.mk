################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/OSAL/POSIX/OSAL.cpp 

OBJS += \
./src/OSAL/POSIX/OSAL.o 

CPP_DEPS += \
./src/OSAL/POSIX/OSAL.d 


# Each subdirectory must supply rules for building sources it contributes
src/OSAL/POSIX/%.o: ../src/OSAL/POSIX/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -I"/Users/cspinner/Documents/workspace/pigpio" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I"/Users/cspinner/Documents/workspace/SpnQC/src/HAL" -I"/Users/cspinner/Documents/workspace/SpnQC/src/OSAL" -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SUPPORT" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SENSE" -I"/Users/cspinner/Documents/workspace/SpnQC/src/CONTROL" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


