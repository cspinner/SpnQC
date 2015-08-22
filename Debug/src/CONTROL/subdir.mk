################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CONTROL/spnMotors.cpp \
../src/CONTROL/spnPID.cpp \
../src/CONTROL/spnServoblaster.cpp 

OBJS += \
./src/CONTROL/spnMotors.o \
./src/CONTROL/spnPID.o \
./src/CONTROL/spnServoblaster.o 

CPP_DEPS += \
./src/CONTROL/spnMotors.d \
./src/CONTROL/spnPID.d \
./src/CONTROL/spnServoblaster.d 


# Each subdirectory must supply rules for building sources it contributes
src/CONTROL/%.o: ../src/CONTROL/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src/MADGWICK" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SENSE" -I"/Users/cspinner/Documents/workspace/SpnQC/src/CONTROL" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


