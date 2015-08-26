/*
 * spnCompass.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnSensor.h"
#include "spnCompass.h"
#include "wiringPiI2C.h"
#include "stdio.h"
using namespace std;

#define CMPS03_ADDRESS 0x60u

// For the CMPS03 sensor
SpnCompass::SpnCompass(void)
:SpnSensor()
{
	i2c_fd = 0;
}

bool SpnCompass::configure(void)
{
	setStatus(SpnSensor::configure());

	if(getStatus() == SUCCESS)
	{
		// I2C
		i2c_fd = wiringPiI2CSetup(CMPS03_ADDRESS);
		version = wiringPiI2CReadReg8 (i2c_fd, 0x0);
	}

	return getStatus();
}

void SpnCompass::acquireData(void)
{
	SpnSensor::acquireData();
	int16_t hi = wiringPiI2CReadReg8(i2c_fd, 2);
	int16_t lo = wiringPiI2CReadReg8(i2c_fd, 3);

	//printf ("Hi: %d, Lo:%d\n", hi, lo);
	dataRaw = (hi << 8) | lo;
}

bool SpnCompass::retrieveData(uint32_t* size, void* data)
{
	setStatus(SpnSensor::retrieveData(size, data));

	if(getStatus() == SUCCESS)
	{
		*size = 1;
		*(float32_t*)data = dataRaw/10.0;
	}

	return getStatus();
}


