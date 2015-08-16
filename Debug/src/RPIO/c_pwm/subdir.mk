################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/RPIO/c_pwm/pwm.c 

OBJS += \
./src/RPIO/c_pwm/pwm.o 

C_DEPS += \
./src/RPIO/c_pwm/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
src/RPIO/c_pwm/%.o: ../src/RPIO/c_pwm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-linux-gnueabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


