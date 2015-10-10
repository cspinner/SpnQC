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
	@echo 'Invoking: GCC C++ Compiler'
	g++ -IZ:\Documents\workspace\pigpio -IZ:\Documents\workspace\SpnQC\src -IZ:\Documents\workspace\SpnQC\src\HAL -IZ:\Documents\workspace\SpnQC\src\OSAL -IZ:\Documents\workspace\wiringpi\include -IZ:\Documents\workspace\SpnQC\src\SUPPORT -IZ:\Documents\workspace\SpnQC\src\SENSE -IZ:\Documents\workspace\SpnQC\src\CONTROL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


