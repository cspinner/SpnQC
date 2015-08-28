/*
 * spnPID.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnPID.h"
#include <math.h>

using namespace std;

#define FLT_EPSILON 0.0001f

SpnPID::SpnPID(void)
{
	Kp = 1.0;
	Ki = 0.0;
	Kd = 0.0;
}

bool SpnPID::configure(float32_t minOut, float32_t maxOut, float32_t interval,
					   float32_t KpIn, float32_t KiIn, float32_t KdIn)
{
	if(interval > 0.0)
	{
		MinOut = minOut;
		MaxOut = maxOut;
		Interval = interval;
		Kp = KpIn;
		Ki = KiIn;
		Kd = KdIn;

		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

void SpnPID::setGains(float32_t KpIn, float32_t KiIn, float32_t KdIn)
{
	Kp = KpIn;
	Ki = KiIn;
	Kd = KdIn;
}

float32_t SpnPID::update(float32_t setPoint, float32_t feedback)
{
	static float32_t errorPrev = 0;
	static float32_t integral = 0;

	float32_t error;
	float32_t derivative;
	float32_t output;

	// Calculate P, I and D terms
	error = setPoint - feedback;

	// In case of error too small then stop integration
	if(fabsf(error) > FLT_EPSILON)
	{
		integral = integral + error*Interval;
		integral = clamp(integral, MinOut, MaxOut);
	}

	derivative = (error - errorPrev)/Interval;

	output = Kp*error + Ki*integral + Kd*derivative;

	// Saturation Filter
	if(output > MaxOut)
	{
		output = MaxOut;
	}
	else if(output < MinOut)
	{
		output = MinOut;
	}

	// Update error
	errorPrev = error;

	return output;
}
