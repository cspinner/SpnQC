################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HAL/QC/HAL.cpp \
../src/HAL/QC/HAL_IMU.cpp \
../src/HAL/QC/HAL_LED.cpp \
../src/HAL/QC/HAL_SERVO.cpp \
../src/HAL/QC/nineAxisMotionSensor.cpp \
../src/HAL/QC/spnSensor.cpp 

OBJS += \
./src/HAL/QC/HAL.o \
./src/HAL/QC/HAL_IMU.o \
./src/HAL/QC/HAL_LED.o \
./src/HAL/QC/HAL_SERVO.o \
./src/HAL/QC/nineAxisMotionSensor.o \
./src/HAL/QC/spnSensor.o 

CPP_DEPS += \
./src/HAL/QC/HAL.d \
./src/HAL/QC/HAL_IMU.d \
./src/HAL/QC/HAL_LED.d \
./src/HAL/QC/HAL_SERVO.d \
./src/HAL/QC/nineAxisMotionSensor.d \
./src/HAL/QC/spnSensor.d 


# Each subdirectory must supply rules for building sources it contributes
src/HAL/QC/%.o: ../src/HAL/QC/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-linux-gnueabi-g++ -DNOCONSOLE -I"/Users/cspinner/Documents/workspace/pigpio" -I"/Users/cspinner/Documents/workspace/wiringPi/wiringPi/include" -I"/Users/cspinner/Documents/workspace/SpnQC/src" -I"/Users/cspinner/Documents/workspace/SpnQC/src/APP" -I"/Users/cspinner/Documents/workspace/SpnQC/src/APP/SUPPORT" -I"/Users/cspinner/Documents/workspace/SpnQC/src/HAL" -I"/Users/cspinner/Documents/workspace/SpnQC/src/OSAL" -I/Volumes/xtools/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.8.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


