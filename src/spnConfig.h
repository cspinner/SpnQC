/*
 * spnConfig.h
 *
 *  Created on: Aug 23, 2015
 *      Author: cspinner
 */

#ifndef SPNCONFIG_H_
#define SPNCONFIG_H_

#include <stdint.h>

#define USER_GPIO_MAX 32

typedef float  float32_t;
typedef double float64_t;

typedef struct
{
	struct
	{
		uint32_t chipSelect; // 0 or 1
		uint32_t speed; // Hz
	} spi;

	struct
	{
		// These biases will be added to input data
		struct
		{
			float32_t x_bias; // g
			float32_t y_bias; // g
			float32_t z_bias; // g
		} accel;

		// These biases will be added to input data
		struct
		{
			float32_t x_bias; // º/s
			float32_t y_bias; // º/s
			float32_t z_bias; // º/s
		} gyro;

		struct
		{
			// compensates for hard errors:
			float32_t x_bias; // mG
			float32_t y_bias; // mG
			float32_t z_bias; // mG

			// compensates for soft errors:
			float32_t x_scale;
			float32_t y_scale;
			float32_t z_scale;
		} mag;

		float32_t beta;
		uint32_t accFsSel; // 0 - 3
		uint32_t gyroFsSel; // 0 - 3
		float32_t magOutlierThresh;
		uint32_t rollingAvgCount; // 1 - 128
	} mpu9250;

	struct
	{
		uint32_t chanCount; // 0 - 32
		uint32_t gpioPin[USER_GPIO_MAX]; // BCM number
        bool useTerminal; 
	} transceiver;

	struct
	{
		uint32_t chanCount; // 0 - 32
		uint32_t gpioPin[USER_GPIO_MAX]; // BCM number
        float32_t pulseWidthZeroThrottle;
        float32_t pulseWidthFullThrottle;
	} motor;

	struct
	{
		float32_t pidOutMin; // %
		float32_t pidOutMax; // %
		float32_t pidPitchKp;
		float32_t pidPitchKi;
		float32_t pidPitchKd;
		float32_t pidRollKp;
		float32_t pidRollKi;
		float32_t pidRollKd;
		float32_t pidYawKp;
		float32_t pidYawKi;
		float32_t pidYawKd;
	} command;
} SpnQC_Config_Type;

bool spnConfigInit(void);
const SpnQC_Config_Type* const spnConfigGet(void);




#endif /* SPNCONFIG_H_ */
