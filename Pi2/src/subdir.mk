################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/spnCommand.cpp \
../src/spnConfig.cpp \
../src/spnHandlers.cpp \
../src/spnInit.cpp \
../src/spnMode.cpp \
../src/spnScheduler.cpp \
../src/spnSensorManager.cpp \
../src/spnServer.cpp \
../src/spnUserInput.cpp \
../src/spnUserOutput.cpp \
../src/spnUtils.cpp 

OBJS += \
./src/main.o \
./src/spnCommand.o \
./src/spnConfig.o \
./src/spnHandlers.o \
./src/spnInit.o \
./src/spnMode.o \
./src/spnScheduler.o \
./src/spnSensorManager.o \
./src/spnServer.o \
./src/spnUserInput.o \
./src/spnUserOutput.o \
./src/spnUtils.o 

CPP_DEPS += \
./src/main.d \
./src/spnCommand.d \
./src/spnConfig.d \
./src/spnHandlers.d \
./src/spnInit.d \
./src/spnMode.d \
./src/spnScheduler.d \
./src/spnSensorManager.d \
./src/spnServer.d \
./src/spnUserInput.d \
./src/spnUserOutput.d \
./src/spnUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -I"/Users/cspinner/Documents/workspace/pigpio" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I"/Users/cspinner/Documents/workspace/SpnQC/src/HAL" -I"/Users/cspinner/Documents/workspace/SpnQC/src/OSAL" -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SUPPORT" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SENSE" -I"/Users/cspinner/Documents/workspace/SpnQC/src/CONTROL" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


