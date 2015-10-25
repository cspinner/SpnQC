/*
 * spnMode.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include <stdlib.h>

const char* GBL_MODE_STRINGS[MODE_COUNT_E] =
{
		//MODE_INIT_E:
		"INITIALIZATION MODE",

		//MODE_ESTABLISH_COMM_E:
		"ESTABLISH COMM MODE",

		//MODE_STANDBY_E:
		"STANDBY MODE",

		//MODE_RUN_E:
		"RUN MODE",

		//MODE_STOP_E:
		"STOP MODE",

		//MODE_CALIBRATE_E:
		"CALIBRATION MODE",

		//MODE_LOST_COMM_E:
		"LOST COMMUNICATION"
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
			if(spnInitCompleted())
			{
				spnGlobalMode = MODE_ESTABLISH_COMM_E;
			}
			break;

		case MODE_ESTABLISH_COMM_E:
			if(spnUserInputCommEstablished())
			{
				spnGlobalMode = MODE_STANDBY_E;
			}
			break;

		case MODE_STANDBY_E:
			if((userInput == 'z' ) || (userInput == 'Z'))
			{
				spnGlobalMode = MODE_CALIBRATE_E;
			}
			// check for stop requests
			if(((userInput == 's' ) || (userInput == 'S')))
			{
				spnGlobalMode = MODE_STOP_E;
			}

			// check for disconnection - not necessarily an error
			if(spnUserInputCommEstablished() == false)
			{
				spnGlobalMode = MODE_ESTABLISH_COMM_E;
			}
			break;

		case MODE_RUN_E:
		case MODE_CALIBRATE_E:
			// check for stop requests
			if(((userInput == 's' ) || (userInput == 'S')))
			{
				spnGlobalMode = MODE_STOP_E;
			}
			if((userInput == 'r' ) || (userInput == 'R'))
			{
				spnGlobalMode = MODE_RUN_E;
			}

			// check for disconnection - error condition
			if(spnUserInputCommEstablished() == false)
			{
				spnGlobalMode = MODE_LOST_COMM_E;
			}
			break;

		case MODE_LOST_COMM_E:
			spnGlobalMode = MODE_LOST_COMM_E;
			break;

		case MODE_STOP_E:
		{
			spnGlobalMode = MODE_STOP_E;

			// check for disconnection - not necessarily an error
			if(spnUserInputCommEstablished() == false)
			{
				spnGlobalMode = MODE_ESTABLISH_COMM_E;
			}
		}
		break;

		default:
			spnGlobalMode = MODE_STOP_E;
			break;
	}

}

