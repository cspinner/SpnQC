/*
 * HAL.h
 *
 *  Created on: Sep 26, 2015
 *      Author: cspinner
 */

#ifndef HAL_QC_HAL_H_
#define HAL_QC_HAL_H_

#include <stdint.h>

#define LED_ON 1
#define LED_OFF 0

enum
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
	NUM_AXIS,
};

bool HAL_INIT(void);

bool HAL_STATUS_LED_SET(uint32_t state);

bool HAL_IMU_INIT(void);
bool HAL_IMU_UPDATE(void);
bool HAL_IMU_READ(float* accel, float* gyro, float* mag);
bool HAL_IMU_READ_RAW(float* accel, float* gyro, float* mag, uint32_t index);
bool HAL_IMU_TEMPERATURE_GET(float* temperature);

bool HAL_SERVO_INPUT_INIT(uint32_t inputCount, const uint32_t* gpioInputList);
bool HAL_SERVO_OUTPUT_INIT(uint32_t outputCount, const uint32_t* gpioOutputList);
bool HAL_SERVO_PULSE_WIDTH_GET(uint32_t gpioIndex, uint32_t* pPulseWidth);
bool HAL_SERVO_PULSE_WIDTH_SET(uint32_t gpioIndex, uint32_t pulseWidthUsec);
bool HAL_SERVO_GET_COMMANDED_PULSE_WIDTH(uint32_t gpioIndex, uint32_t* pPulseWidth);
bool HAL_SERVO_PULSES_STOP(void);

#endif /* HAL_QC_HAL_H_ */
