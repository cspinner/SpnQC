################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/spnHandlers.cpp \
../src/spnInit.cpp \
../src/spnMode.cpp \
../src/spnMotorManager.cpp \
../src/spnScheduler.cpp \
../src/spnSensorManager.cpp \
../src/spnStatus.cpp \
../src/spnUserOutput.cpp \
../src/spnUtils.cpp 

OBJS += \
./src/main.o \
./src/spnHandlers.o \
./src/spnInit.o \
./src/spnMode.o \
./src/spnMotorManager.o \
./src/spnScheduler.o \
./src/spnSensorManager.o \
./src/spnStatus.o \
./src/spnUserOutput.o \
./src/spnUtils.o 

CPP_DEPS += \
./src/main.d \
./src/spnHandlers.d \
./src/spnInit.d \
./src/spnMode.d \
./src/spnMotorManager.d \
./src/spnScheduler.d \
./src/spnSensorManager.d \
./src/spnStatus.d \
./src/spnUserOutput.d \
./src/spnUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src/MADGWICK" -I"/Users/cspinner/Documents/workspace/SpnQC/src/SENSE" -I"/Users/cspinner/Documents/workspace/SpnQC/src/CONTROL" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


