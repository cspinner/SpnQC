/*
 * HAL.cpp
 *
 *  Created on: Sep 26, 2015
 *      Author: cspinner
 */

#include "HAL_private.h"
#include "wiringpi.h"
#include <stdlib.h>
#include <stdint.h>

bool HAL_INIT(void)
{
	if((piHiPri(99) == 0) && (wiringPiSetup() == 0))
	{
		pinMode (OUTPUT_PIN_STATUS_LED, OUTPUT) ; // LED

		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}
