/*
 * spnMode.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: cspinner
 */

#include "spnQC.h"

const char* MODE_STRINGS[MODE_COUNT_E] =
{
		//MODE_INIT_E:
		"INITIALIZATION MODE",

		//MODE_TEST_E:
		"TEST MODE",

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
	return MODE_STRINGS[spnGlobalMode];
}

System_Mode_Type spnModeGet(void)
{
	return spnGlobalMode;
}

void spnModeSet(System_Mode_Type mode)
{
	// Restrict mode transitions
	switch(spnGlobalMode)
	{
		case MODE_INIT_E:
			spnGlobalMode = mode;
			break;

		// Do not allow transition to any mode other than stop
		case MODE_CALIBRATE_E:
		case MODE_TEST_E:
		case MODE_STOP_E:
		case MODE_RUN_E:
		default:
			if(mode == MODE_STOP_E)
			{
				spnGlobalMode = mode;
			}
			break;
	}

}

