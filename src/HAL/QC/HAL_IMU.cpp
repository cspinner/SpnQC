/*
 * HAL_IMU.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */

#include "spnConfig.h"
#include "spnNineAxisMotion.h"
#include <stdlib.h>
#include <stdint.h>

static SpnNineAxisMotion MPU9250;
static float Temperature;


bool HAL_IMU_INIT(void)
{
	const SpnQC_Config_Type* const pCfg = spnConfigGet();

	SpnNineAxisMotion_Cfg_Type cfg_mpu9250;

	// Copy config data
	cfg_mpu9250.chipSelect = pCfg->spi.chipSelect;
	cfg_mpu9250.speed = pCfg->spi.speed;
	cfg_mpu9250.accFsSel = pCfg->mpu9250.accFsSel;
	cfg_mpu9250.gyroFsSel = pCfg->mpu9250.gyroFsSel;
	cfg_mpu9250.magOutlierThresh = pCfg->mpu9250.magOutlierThresh;

	for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
	{
		cfg_mpu9250.calibration.accel[axis] = pCfg->mpu9250.accel[axis];
		cfg_mpu9250.calibration.gyro[axis] = pCfg->mpu9250.gyro[axis];
		cfg_mpu9250.calibration.magb[axis] = pCfg->mpu9250.magb[axis];
		cfg_mpu9250.calibration.mags[axis] = pCfg->mpu9250.mags[axis];
	}

	return MPU9250.configure(&cfg_mpu9250);
}

bool HAL_IMU_UPDATE(void)
{
	MPU9250.acquireData();

	return EXIT_SUCCESS;
}

bool HAL_IMU_READ(float* accel, float* gyro, float* mag)
{
	uint32_t phNineAxisDataSize;
	SpnNineAxisMotion_Data_Type sensorData;

	// Convert Sensor Data
	MPU9250.retrieveData(&phNineAxisDataSize, &sensorData);

	// Buffer to output memory
	for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
	{
		accel[axis] = sensorData.accel[axis];
		gyro[axis] = sensorData.gyro[axis];
		mag[axis] = sensorData.mag[axis];
	}

	Temperature = sensorData.temperature;

	return EXIT_SUCCESS;
}

bool HAL_IMU_READ_RAW(float* accel, float* gyro, float* mag, uint32_t index)
{
	uint32_t phNineAxisDataSize;
	SpnNineAxisMotion_Data_Type sensorData;

	MPU9250.retrieveData((void*)index, &phNineAxisDataSize, &sensorData);

	// Buffer to output memory
	for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
	{
		accel[axis] = sensorData.accel[axis];
		gyro[axis] = sensorData.gyro[axis];
		mag[axis] = sensorData.mag[axis];
	}

	return EXIT_SUCCESS;
}

bool HAL_IMU_TEMPERATURE_GET(float* temperature)
{
	*temperature = Temperature;

	return EXIT_SUCCESS;
}


