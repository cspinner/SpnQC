################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/OSAL/WIN/OSAL.cpp \
../src/OSAL/WIN/OSAL_TIME.cpp 

OBJS += \
./src/OSAL/WIN/OSAL.o \
./src/OSAL/WIN/OSAL_TIME.o 

CPP_DEPS += \
./src/OSAL/WIN/OSAL.d \
./src/OSAL/WIN/OSAL_TIME.d 


# Each subdirectory must supply rules for building sources it contributes
src/OSAL/WIN/%.o: ../src/OSAL/WIN/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -I"/Users/cspinner/Documents/workspace/pigpio" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I"/Users/cspinner/Documents/workspace/SpnQC/src/HAL" -I"/Users/cspinner/Documents/workspace/SpnQC/src/OSAL" -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SUPPORT" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SENSE" -I"/Users/cspinner/Documents/workspace/SpnQC/src/CONTROL" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


