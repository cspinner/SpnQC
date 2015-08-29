/*
 * spnInit.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "wiringPi.h"
#include <stdlib.h>

using namespace std;

bool spnInit(void)
{
	bool status = EXIT_FAILURE;

	// Attempt to increase program's scheduling priority
	if(piHiPri(99) == 0)
	{
		// Retrieve configuration
		if(spnConfigInit() == EXIT_SUCCESS)
		{
			// Configure CPU peripherals
			wiringPiSetup() ;
			pinMode (OUTPUT_PIN_STATUS_LED, OUTPUT) ; // LED

			// Configure sensors, motors, console/file output
			if( (spnSensorManagerInit() == EXIT_SUCCESS) &&
				(spnCommandInit() == EXIT_SUCCESS) &&
				(spnUserOutputInit() == EXIT_SUCCESS) &&
				(spnUserInputInit() == EXIT_SUCCESS))
			{
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

