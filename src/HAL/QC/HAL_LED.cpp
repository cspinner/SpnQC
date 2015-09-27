/*
 * HAL_LED.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */

#include "HAL_private.h"
#include "wiringpi.h"
#include <stdint.h>
#include <stdlib.h>

bool HAL_STATUS_LED_SET(uint32_t state)
{
	digitalWrite (OUTPUT_PIN_STATUS_LED, state);

	return EXIT_SUCCESS;
}


