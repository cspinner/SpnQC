/*
 * spnMotorManager.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnPID.h"

#define PR_RATE_MIN_DEGPS -5.0
#define PR_RATE_MAX_DEGPS  5.0

SpnPID PitchRatePID;
SpnPID PitchAnglePID;
SpnPID RollRatePID;
SpnPID RollAnglePID;

bool spnMotorManagerInit(void)
{
	float pid_interval = MINOR_FRAME_TIME_USEC/1000000.0;

	// Configure controller PIDs
	if( (PitchRatePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS) &&
	    (PitchAnglePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS) &&
		(RollRatePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS) &&
		(RollAnglePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS))
	{
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}
float SpnPitchAngleSense;
float SpnPitchRateSense;
void spnMotorManagerUpdate(void)
{
	float pitchRateCommand = PitchAnglePID.update(0.0, SpnPitchAngleSense);
	float motorCommand = PitchRatePID.update(pitchRateCommand, SpnPitchRateSense);

}



