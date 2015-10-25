################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/APP/main.cpp \
../src/APP/spnCommand.cpp \
../src/APP/spnConfig.cpp \
../src/APP/spnFilter.cpp \
../src/APP/spnHandlers.cpp \
../src/APP/spnInit.cpp \
../src/APP/spnMode.cpp \
../src/APP/spnMotors.cpp \
../src/APP/spnPID.cpp \
../src/APP/spnScheduler.cpp \
../src/APP/spnSensorManager.cpp \
../src/APP/spnServer.cpp \
../src/APP/spnTransceiver.cpp \
../src/APP/spnUserInput.cpp \
../src/APP/spnUserOutput.cpp \
../src/APP/spnUtils.cpp 

OBJS += \
./src/APP/main.o \
./src/APP/spnCommand.o \
./src/APP/spnConfig.o \
./src/APP/spnFilter.o \
./src/APP/spnHandlers.o \
./src/APP/spnInit.o \
./src/APP/spnMode.o \
./src/APP/spnMotors.o \
./src/APP/spnPID.o \
./src/APP/spnScheduler.o \
./src/APP/spnSensorManager.o \
./src/APP/spnServer.o \
./src/APP/spnTransceiver.o \
./src/APP/spnUserInput.o \
./src/APP/spnUserOutput.o \
./src/APP/spnUtils.o 

CPP_DEPS += \
./src/APP/main.d \
./src/APP/spnCommand.d \
./src/APP/spnConfig.d \
./src/APP/spnFilter.d \
./src/APP/spnHandlers.d \
./src/APP/spnInit.d \
./src/APP/spnMode.d \
./src/APP/spnMotors.d \
./src/APP/spnPID.d \
./src/APP/spnScheduler.d \
./src/APP/spnSensorManager.d \
./src/APP/spnServer.d \
./src/APP/spnTransceiver.d \
./src/APP/spnUserInput.d \
./src/APP/spnUserOutput.d \
./src/APP/spnUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/APP/%.o: ../src/APP/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -I"/Users/cspinner/Documents/workspace/pigpio" -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I"/Users/cspinner/Documents/workspace/SpnQC/src/APP" -I"/Users/cspinner/Documents/workspace/SpnQC/src/APP/SUPPORT" -I"/Users/cspinner/Documents/workspace/SpnQC/src/HAL" -I"/Users/cspinner/Documents/workspace/SpnQC/src/OSAL" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


