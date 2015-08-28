/*
 * spnMotors.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"
#include <stdlib.h>

// y = mc + b  --- linear relationship between command % and pulse width
// commanded pulse width = slope * command percent + intercept
static float32_t slope;
static float32_t intercept; 
static uint32_t motorCount;

bool spnMotorsInit(void)
{
    const SpnQC_Config_Type* const pCfg = spnConfigGet();
    
    slope = (pCfg->motor.pulseWidthFullThrottle - pCfg->motor.pulseWidthZeroThrottle)/100.0;
    intercept = pCfg->motor.pulseWidthZeroThrottle;
    motorCount = pCfg->motor.chanCount;
    
    atexit(&spnMotorsStopAll);
    
	return SUCCESS;
}

void spnMotorsSet(uint32_t motorNum, float32_t cmdPct)
{
    // calculate pulse width based on commanded percent
	uint32_t pulseWidthUsec = (uint32_t)(slope*cmdPct + intercept);

	// command the servo driver
	spnServoSetPulseWidth(motorNum, pulseWidthUsec);
}

// Get motor pct 0-100%
float32_t spnMotorsGet(uint32_t motorNum)
{
	uint32_t pulseWidthUsec = spnServoGetCommandedPulseWidth(motorNum);
	float32_t cmdPct = (((float32_t)pulseWidthUsec - intercept)/slope);

	return clamp(cmdPct, 0.0, 100.0);
}

// 0 = high, 1 = center, 2 = low
void spnMotorsCalibrateDrive(uint32_t level)
{
	const uint32_t pulseWidths[] = {
			2000, 1500, 1000
	};

	if(level <= 2)
	{
		for(uint32_t i = 0; i < motorCount; i++)
		{
			spnServoSetPulseWidth(i, pulseWidths[level]);
		}
	}
	else
	{
		spnServoStopAllPulses();
	}
}

void spnMotorsStopAll(void)
{
	printf("Stopping all motors...\n");
	spnServoStopAllPulses();
}


