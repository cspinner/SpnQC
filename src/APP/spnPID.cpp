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
	m_Kp = 0;
	m_Ki = 0;
	m_Kd = 0;
	m_MinOut = 0;
	m_MaxOut = 0;
	m_Proportional = 0;
	m_Interval = 0;
	m_Derivative = 0;
	m_Output = 0;
	m_ErrorPrev = 0;
	m_Integral = 0;
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

void SpnPID::getGains(float32_t* KpOut, float32_t* KiOut, float32_t* KdOut)
{
    *KpOut = m_Kp;
    *KiOut = m_Ki;
    *KdOut = m_Kd;
}

void SpnPID::getTerms(float32_t* pOut, float32_t* iOut, float32_t* dOut, float32_t* sumOut)
{
	*pOut = m_Proportional;
	*iOut = m_Integral;
	*dOut = m_Derivative;
	*sumOut = m_Output;
}

float32_t SpnPID::update(float32_t setPoint, float32_t feedback)
{
	float32_t error;

	// Calculate P, I and D terms
	error = setPoint - feedback;

	// In case of error too small then stop integration
	if(fabsf(error) > FLT_EPSILON)
	{
		m_Integral = m_Integral + error*m_Interval;
		m_Integral = clamp(m_Integral, m_MinOut/m_Ki, m_MaxOut/m_Ki);
	}

	m_Derivative = (error - m_ErrorPrev)/m_Interval;
	m_Proportional = error;

	m_Output = m_Kp*m_Proportional + m_Ki*m_Integral + m_Kd*m_Derivative;

	// Saturation Filter
	if(m_Output > m_MaxOut)
	{
		m_Output = m_MaxOut;
	}
	else if(m_Output < m_MinOut)
	{
		m_Output = m_MinOut;
	}

	// Update error
	m_ErrorPrev = error;

	return m_Output;
}
