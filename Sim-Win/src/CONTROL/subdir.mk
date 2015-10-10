################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CONTROL/spnMotors.cpp \
../src/CONTROL/spnPID.cpp \
../src/CONTROL/spnTransceiver.cpp 

OBJS += \
./src/CONTROL/spnMotors.o \
./src/CONTROL/spnPID.o \
./src/CONTROL/spnTransceiver.o 

CPP_DEPS += \
./src/CONTROL/spnMotors.d \
./src/CONTROL/spnPID.d \
./src/CONTROL/spnTransceiver.d 


# Each subdirectory must supply rules for building sources it contributes
src/CONTROL/%.o: ../src/CONTROL/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -IZ:\Documents\workspace\pigpio -IZ:\Documents\workspace\SpnQC\src -IZ:\Documents\workspace\SpnQC\src\HAL -IZ:\Documents\workspace\SpnQC\src\OSAL -IZ:\Documents\workspace\wiringpi\include -IZ:\Documents\workspace\SpnQC\src\SUPPORT -IZ:\Documents\workspace\SpnQC\src\SENSE -IZ:\Documents\workspace\SpnQC\src\CONTROL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


