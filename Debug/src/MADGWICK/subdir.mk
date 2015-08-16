################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MADGWICK/MadgwickAHRS.c 

OBJS += \
./src/MADGWICK/MadgwickAHRS.o 

C_DEPS += \
./src/MADGWICK/MadgwickAHRS.d 


# Each subdirectory must supply rules for building sources it contributes
src/MADGWICK/%.o: ../src/MADGWICK/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-linux-gnueabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


