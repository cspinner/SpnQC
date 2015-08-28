/*
 * spnMode.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: cspinner
 */

#include "spnQC.h"

const char* GBL_MODE_STRINGS[MODE_COUNT_E] =
{
		//MODE_INIT_E:
		"INITIALIZATION MODE",

		//MODE_STANDBY_E:
		"STANDBY MODE",

		//MODE_RUN_E:
		"RUN MODE",

		//MODE_STOP_E:
		"STOP MODE",

		//MODE_CALIBRATE_E:
		"CALIBRATION MODE"
};

static System_Mode_Type spnGlobalMode = MODE_INIT_E;

const char* spnModeGetString(void)
{
	return GBL_MODE_STRINGS[spnGlobalMode];
}

System_Mode_Type spnModeGet(void)
{
	return spnGlobalMode;
}

void spnModeUpdate(void)
{
	char userInput = spnUserInputCharGet(false);

	// Manage mode transitions
	switch(spnGlobalMode)
	{
		case MODE_INIT_E:
			spnGlobalMode = MODE_STANDBY_E;
			break;

		case MODE_STANDBY_E:
			if((spnSchedulerGetFrameCount()*MINOR_FRAME_TIME_USEC)/1000000 > 10)
			{
				spnGlobalMode = MODE_RUN_E;
			}
			else if((userInput == 'z' ) || (userInput == 'Z'))
			{
				spnGlobalMode = MODE_CALIBRATE_E;
			}
			break;

		case MODE_RUN_E:
		case MODE_CALIBRATE_E:
			if((userInput == 's' ) || (userInput == 'S'))
			{
				spnGlobalMode = MODE_STOP_E;
			}
			if((userInput == 'r' ) || (userInput == 'R'))
			{
				spnGlobalMode = MODE_RUN_E;
			}
			break;

		case MODE_STOP_E:
		default:
			spnGlobalMode = MODE_STOP_E;
			break;
	}

}

