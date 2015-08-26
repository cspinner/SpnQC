/*
 * spnHandlers.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef SIGALRM_DEBUG
#include <sys/time.h>

static void sigalrm_debug(void);
#endif

#define CALLBACKS_MAX 32

static __sighandler_t pCallbacks[CALLBACKS_MAX];
static uint32_t CallbackIndex = 0;

void spnHandleRegisterCallback(__sighandler_t pCallback)
{
	// Register the callback
	if(CallbackIndex < CALLBACKS_MAX)
	{
		pCallbacks[CallbackIndex] = pCallback;
		CallbackIndex++;
	}
}

void spnHandleSignal(int32_t signum)
{
	// Invoke each of the predefined callbacks
	for(uint32_t i = 0; i < CallbackIndex; i++)
	{
		if(pCallbacks[i])
		{
			pCallbacks[i](signum);
		}
	}

	switch(signum)
	{
		case SIGALRM:
#ifdef SIGALRM_DEBUG
			sigalrm_debug();
#endif
			spnSchedulerForeground();
			break;

		case SIGINT:
		case SIGQUIT:
		case SIGTSTP:
			printf("\n\nExiting...\n");

			// Now reset to the default signal action
			signal(signum, SIG_DFL);
			exit(0);
			break;

		default:
			printf("Unexpected signal: %d", signum);
			spnStatusSet(FAIL);
			break;
	}
}

#ifdef SIGALRM_DEBUG
static void sigalrm_debug(void)
{
	static struct timeval tsPrev;
	static uint32_t cnt = 0;

	if (cnt == 0)
	{
		gettimeofday (&tsPrev, 0);
	}
	else
	{
		struct timeval tsNow;
		struct timeval diff;

		gettimeofday (&tsNow, 0);
		timersub(&tsNow, &tsPrev, &diff);
		tsPrev = tsNow;
		printf("Diff: %d\n", diff.tv_usec);
	 }
	cnt++;
}
#endif
