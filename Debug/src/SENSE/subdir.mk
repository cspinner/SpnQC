################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SENSE/spnCompass.cpp \
../src/SENSE/spnNineAxisMotion.cpp \
../src/SENSE/spnSensor.cpp 

OBJS += \
./src/SENSE/spnCompass.o \
./src/SENSE/spnNineAxisMotion.o \
./src/SENSE/spnSensor.o 

CPP_DEPS += \
./src/SENSE/spnCompass.d \
./src/SENSE/spnNineAxisMotion.d \
./src/SENSE/spnSensor.d 


# Each subdirectory must supply rules for building sources it contributes
src/SENSE/%.o: ../src/SENSE/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -I"/Users/cspinner/Documents/workspace/pigpio" -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src/MADGWICK" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SENSE" -I"/Users/cspinner/Documents/workspace/SpnQC/src/CONTROL" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


