/*
 * spnMotors.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"
#include <stdlib.h>

#define MOTORS_COUNT 4

// List of output pins using BCM number (list ends with -1)
const int outputPins[] = { 4, 17, 22, 27, -1 };

// List of input pins using BCM number (list ends with -1)
const int inputPins[] = { 23, -1 };

bool spnMotorsInit(void)
{
	return spnServoInit(&inputPins[0], &outputPins[0]);
}

void spnMotorsSet(int motorNum, float cmdPct)
{
	// 1050 to 2050
	int pulseWidthUsec = (int)(10.0*cmdPct + 1050.0);

	cmdPct = clamp(cmdPct, 0.0, 100.0);

	// command the servo driver
	spnServoSetPulseWidth(motorNum, pulseWidthUsec);
}

// Get motor pct 0-100%
float spnMotorsGet(int motorNum)
{
	int pulseWidthUsec = spnServoGetCommandedPulseWidth(motorNum);
	float cmdPct = (((float)pulseWidthUsec - 1050.0)/10.0);

	return clamp(cmdPct, 0.0, 100.0);
}

// 0 = high, 1 = center, 2 = low
void spnMotorsCalibrateDrive(int level)
{
	int pulseWidths[] = {
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


