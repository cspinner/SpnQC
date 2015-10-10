################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HAL/QC/HAL.cpp \
../src/HAL/QC/HAL_IMU.cpp \
../src/HAL/QC/HAL_LED.cpp \
../src/HAL/QC/HAL_SERVO.cpp 

OBJS += \
./src/HAL/QC/HAL.o \
./src/HAL/QC/HAL_IMU.o \
./src/HAL/QC/HAL_LED.o \
./src/HAL/QC/HAL_SERVO.o 

CPP_DEPS += \
./src/HAL/QC/HAL.d \
./src/HAL/QC/HAL_IMU.d \
./src/HAL/QC/HAL_LED.d \
./src/HAL/QC/HAL_SERVO.d 


# Each subdirectory must supply rules for building sources it contributes
src/HAL/QC/%.o: ../src/HAL/QC/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -IZ:\Documents\workspace\pigpio -IZ:\Documents\workspace\SpnQC\src -IZ:\Documents\workspace\SpnQC\src\HAL -IZ:\Documents\workspace\SpnQC\src\OSAL -IZ:\Documents\workspace\wiringpi\include -IZ:\Documents\workspace\SpnQC\src\SUPPORT -IZ:\Documents\workspace\SpnQC\src\SENSE -IZ:\Documents\workspace\SpnQC\src\CONTROL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


