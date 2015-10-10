#include "spnQC.h"
#include "OSAL.h"
#include <stdlib.h>

using namespace std;

int32_t main (void)
{
	if((OSAL_INIT() == EXIT_SUCCESS) &&
	   (spnHandleInit() == EXIT_SUCCESS) &&
	   (spnInit() == EXIT_SUCCESS))
	{
		// Start the minor frame timer
		spnHandleStartTimer();

		for(;;)
		{
			spnSchedulerPollSensors();
		}

		// Halt the minor frame timer
		spnHandleHaltTimer();

		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}

}

