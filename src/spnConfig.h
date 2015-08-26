/*
 * spnConfig.h
 *
 *  Created on: Aug 23, 2015
 *      Author: cspinner
 */

#ifndef SPNCONFIG_H_
#define SPNCONFIG_H_

#define USER_GPIO_MAX 32

typedef struct
{
	struct
	{
		int chipSelect; // 0 or 1
		int speed; // Hz
	} spi;

	struct
	{
		// These biases will be added to input data
		struct
		{
			float x_bias; // g
			float y_bias; // g
			float z_bias; // g
		} accel;

		// These biases will be added to input data
		struct
		{
			float x_bias; // º/s
			float y_bias; // º/s
			float z_bias; // º/s
		} gyro;

		struct
		{
			// compensates for hard errors:
			float x_bias; // mG
			float y_bias; // mG
			float z_bias; // mG

			// compensates for soft errors:
			float x_scale;
			float y_scale;
			float z_scale;
		} mag;

		float beta;
		int accFsSel; // 0 - 3
		int gyroFsSel; // 0 - 3
		float magOutlierThresh;
		int rollingAvgCount; // 1 - 128
	} mpu9250;

	struct
	{
		int chanCount; // 0 - 32
		int gpioPin[USER_GPIO_MAX]; // BCM number
        bool useTerminal; 
	} transceiver;

	struct
	{
		int chanCount; // 0 - 32
		int gpioPin[USER_GPIO_MAX]; // BCM number
        float pulseWidthZeroThrottle;
        float pulseWidthFullThrottle;
	} motor;

	struct
	{
		float pidOutMin; // %
		float pidOutMax; // %
		float pidPitchKp;
		float pidPitchKi;
		float pidPitchKd;
		float pidRollKp;
		float pidRollKi;
		float pidRollKd;
		float pidYawKp;
		float pidYawKi;
		float pidYawKd;
	} command;
} SpnQC_Config_Type;

bool spnConfigInit(void);
const SpnQC_Config_Type* const spnConfigGet(void);




#endif /* SPNCONFIG_H_ */
