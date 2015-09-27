/*
 * spnInit.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "HAL.h"
#include <stdlib.h>

using namespace std;

bool isInitCompleted = false;

bool spnInit(void)
{
	bool status = EXIT_FAILURE;

	// Attempt to increase program's scheduling priority
	if(HAL_INIT() == EXIT_SUCCESS)
	{
		// Retrieve configuration
		if(spnConfigInit() == EXIT_SUCCESS)
		{
			HAL_STATUS_LED_SET(LED_ON);

			// Configure sensors, motors, console/file output
			if( (spnSensorManagerInit() == EXIT_SUCCESS) &&
				(spnCommandInit() == EXIT_SUCCESS) &&
				(spnUserOutputInit() == EXIT_SUCCESS) &&
				(spnUserInputInit() == EXIT_SUCCESS))
			{
				isInitCompleted = true;

				status = EXIT_SUCCESS;
			}
			else
			{
				status = EXIT_FAILURE;
			}
		}
		else
		{
			status = EXIT_FAILURE;
		}
	}
	else
	{
		status = EXIT_FAILURE;
	}

	return status;
}

bool spnInitCompleted(void)
{
	return isInitCompleted;
}

