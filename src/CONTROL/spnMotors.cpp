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
static float slope;
static float intercept; 

bool spnMotorsInit(void)
{
    const SpnQC_Config_Type* const pCfg = spnConfigGet();
    
    slope = ()pCfg->motors.pulseWidthFullThrottle - pCfg->motors.pulseWidthZeroThrottle)/100.0;
    intercept = pCfg->motors.pulseWidthZeroThrottle;
    
    atexit(&spnMotorsStopAll);
    
	return SUCCESS;
}

void spnMotorsSet(int motorNum, float cmdPct)
{
    // calculate pulse width based on commanded percent
	int pulseWidthUsec = (int)(slope*cmdPct + intercept);

	// command the servo driver
	spnServoSetPulseWidth(motorNum, pulseWidthUsec);
}

// Get motor pct 0-100%
float spnMotorsGet(int motorNum)
{
	int pulseWidthUsec = spnServoGetCommandedPulseWidth(motorNum);
	float cmdPct = (((float)pulseWidthUsec - intercept)/slope);

	return clamp(cmdPct, 0.0, 100.0);
}

// 0 = high, 1 = center, 2 = low
void spnMotorsCalibrateDrive(int level)
{
	const int pulseWidths[] = {
			2000, 1500, 1000
	};

	if(level <= 2)
	{
		for(int i = 0; i < MOTORS_COUNT; i++)
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
	spnServoStopAllPulses();
}


