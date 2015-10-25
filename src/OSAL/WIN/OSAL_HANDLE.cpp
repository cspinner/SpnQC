/*
 * OSAL_HANDLE.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: cspinner
 */

#include "OSAL.h"
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static void (*sigAlarmFunc)(void);

static void handleSignal(int32_t signum);

bool OSAL_HANDLE_INIT(void)
{
	// Register the handler to catch exits
	signal(SIGINT, &handleSignal);
	signal(SIGTERM, &handleSignal);
	signal(SIGABRT, &handleSignal);
	signal(SIGFPE, &handleSignal);
	signal(SIGILL, &handleSignal);
	signal(SIGSEGV, &handleSignal);

	return EXIT_SUCCESS;
}

bool OSAL_HANDLE_SET_ITMR_CB(void (*pCb)(void))
{
	sigAlarmFunc = pCb;

	signal(SIGALRM, &handleSignal);

	return EXIT_SUCCESS;
}

static void handleSignal(int32_t signum)
{
	switch(signum)
	{
		case SIGALRM:
			if(sigAlarmFunc != NULL)
			{
				sigAlarmFunc();
			}
			break;

		case SIGINT:
		case SIGTERM:
			printf("\n\nExiting...\n");

			// Now reset to the default signal action
			signal(signum, SIG_DFL);
			exit(EXIT_SUCCESS);
			break;

		case SIGABRT:
		case SIGFPE:
		case SIGILL:
		case SIGSEGV:
		default:
			printf("Unexpected signal: %d", signum);
			exit(EXIT_FAILURE);
			break;
	}
}





