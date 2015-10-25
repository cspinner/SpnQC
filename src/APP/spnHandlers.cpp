/*
 * spnHandlers.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "OSAL.h"
#include <stdlib.h>
#include <stdint.h>


bool spnHandleInit(void)
{
	PRINT("start spnHandleInit");

	OSAL_HANDLE_INIT();
	OSAL_HANDLE_SET_ITMR_CB(spnSchedulerForeground);

	OSAL_TIME_INT_SET(MINOR_FRAME_TIME_USEC);

	return EXIT_SUCCESS;
}

void spnHandleStartTimer(void)
{
	PRINT("start spnHandleStartTimer");

	// Start the minor frame timer
	OSAL_TIME_INT_START();
}

void spnHandleHaltTimer(void)
{
	// Halt the minor frame timer
	OSAL_TIME_INT_STOP();
}




