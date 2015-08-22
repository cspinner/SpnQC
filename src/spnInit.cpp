/*
 * spnInit.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "wiringPi.h"
#include <iostream>

using namespace std;

bool spnInit(void)
{
	bool status = FAIL;

	// Attempt to increase program's scheduling priority
	if(piHiPri(99) == 0)
	{
		// Configure CPU peripherals
		wiringPiSetup () ;
		pinMode (OUTPUT_PIN_STATUS_LED, OUTPUT) ; // LED
		pinMode (INPUT_PIN_TEST_MODE, INPUT) ; // Test mode switch
		pullUpDnControl(INPUT_PIN_TEST_MODE, PUD_DOWN); // Pulled down => normally 0 when open

		// Configure sensors, motors
		if( (spnSensorManagerInit() == SUCCESS) &&
			(spnCommandInit() == SUCCESS))
		{
			// Configure console/file output
			status = spnUserOutputInit() && spnUserInputInit();
		}
	}
	else
	{
		status = FAIL;
	}

	return status;
}

