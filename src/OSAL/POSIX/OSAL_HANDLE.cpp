/*
 * OSAL_HANDLE.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */

#include "OSAL.h"
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static struct sigaction sa = { 0 };
static void (*sigAlarmFunc)(void);

#ifdef SIGALRM_DEBUG
#include <sys/time.h>

static void sigalrm_debug(void);
#endif

static void handleSignal(int32_t signum);

bool OSAL_HANDLE_INIT(void)
{
	// Register the handler to catch exits
	signal(SIGINT, &handleSignal);
	signal(SIGQUIT, &handleSignal);
	signal(SIGTSTP, &handleSignal);

	// Ignore pipe errors
	signal(SIGPIPE, SIG_IGN);

	return EXIT_SUCCESS;
}

bool OSAL_HANDLE_SET_ITMR_CB(void (*pCb)(void))
{
	sigAlarmFunc = pCb;

	sa.sa_handler = &handleSignal;
	sigaction (SIGALRM, &sa, NULL);

	return EXIT_SUCCESS;
}

static void handleSignal(int32_t signum)
{
	switch(signum)
	{
		case SIGALRM:
#ifdef SIGALRM_DEBUG
			sigalrm_debug();
#endif
			if(sigAlarmFunc != NULL)
			{
				sigAlarmFunc();
			}
			break;

		case SIGINT:
		case SIGQUIT:
		case SIGTSTP:
			printf("\n\nExiting...\n");

			// Now reset to the default signal action
			signal(signum, SIG_DFL);
			exit(EXIT_SUCCESS);
			break;

		default:
			printf("Unexpected signal: %d", signum);
			exit(EXIT_FAILURE);
			break;
	}
}



#ifdef SIGALRM_DEBUG
static void sigalrm_debug(void)
{
	static OSAL_Time_Type tsPrev;
	static uint32_t cnt = 0;

	if (cnt == 0)
	{
		OSAL_TIME_OF_DAY(&tsPrev);
	}
	else
	{
		OSAL_Time_Type tsNow;
		OSAL_Time_Type diff;

		OSAL_TIME_OF_DAY(&tsNow);
		OSAL_TIME_SUB(&tsNow, &tsPrev, &diff);
		tsPrev = tsNow;
		printf("Diff: %d\n", diff.microSeconds);
	 }
	cnt++;
}
#endif
