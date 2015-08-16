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
	Ki = 1.0;
	Kd = 1.0;
}

bool SpnPID::configure(float minOut, float maxOut, float interval)
{
	if(interval > 0.0)
	{
		MinOut = minOut;
		MaxOut = maxOut;
		Interval = interval;

		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

float SpnPID::update(float setPoint, float feedback)
{
	static float errorPrev = 0;
	static float integral = 0;

	float error;
	float derivative;
	float output;

	// Calculate P, I and D terms
	error = setPoint - feedback;

	// In case of error too small then stop integration
	if(fabs(error) > FLT_EPSILON)
	{
		integral = integral + error*Interval;
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
