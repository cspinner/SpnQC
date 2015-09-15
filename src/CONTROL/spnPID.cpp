/*
 * spnPID.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnPID.h"
#include <math.h>
#include <stdlib.h>

using namespace std;

#define FLT_EPSILON 0.0001f

SpnPID::SpnPID(void)
{
	m_ErrorPrev = 0.0;
    m_Integral = 0.0;
}

bool SpnPID::configure(float32_t minOut, float32_t maxOut, float32_t interval,
					   float32_t KpIn, float32_t KiIn, float32_t KdIn)
{
	if(interval > 0.0)
	{
		m_MinOut = minOut;
		m_MaxOut = maxOut;
		m_Interval = interval;
        setGains(KpIn, KiIn, KdIn);

		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

void SpnPID::setGains(float32_t KpIn, float32_t KiIn, float32_t KdIn)
{
	m_Kp = KpIn;
	m_Ki = KiIn;
	m_Kd = KdIn;
}

void getGains(float32_t* KpOut, float32_t* KiOut, float32_t* KdOut)
{
    *KpOut = m_Kp;
    *KiOut = m_Ki;
    *KdOut = m_Kd;
}

float32_t SpnPID::update(float32_t setPoint, float32_t feedback)
{
	float32_t error;
	float32_t derivative;
	float32_t output;

	// Calculate P, I and D terms
	error = setPoint - feedback;

	// In case of error too small then stop integration
	if(fabsf(error) > FLT_EPSILON)
	{
		m_Integral = m_Integral + error*m_Interval;
		m_Integral = clamp(m_Integral, m_MinOut, m_MaxOut);
	}

	derivative = (error - m_ErrorPrev)/m_Interval;

	output = Kp*error + Ki*m_Integral + Kd*derivative;

	// Saturation Filter
	if(output > m_MaxOut)
	{
		output = m_MaxOut;
	}
	else if(output < m_MinOut)
	{
		output = m_MinOut;
	}

	// Update error
	m_ErrorPrev = error;

	return output;
}
