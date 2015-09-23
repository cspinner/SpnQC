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
#include <sys/time.h>

#ifdef SIGALRM_DEBUG
#include <sys/time.h>

static void sigalrm_debug(void);
#endif

#define CALLBACKS_MAX 32

static __sighandler_t pCallbacks[CALLBACKS_MAX];
static uint32_t CallbackIndex = 0;
static struct sigaction sa = { 0 };
static struct itimerval timer = { 0 };

bool spnHandleInit(void)
{
	// Set the minor frame timer
	sa.sa_handler = &spnHandleSignal;
	sigaction (SIGALRM, &sa, NULL);
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = MINOR_FRAME_TIME_USEC;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = MINOR_FRAME_TIME_USEC;

	// Register the handler to catch exits
	signal(SIGINT, &spnHandleSignal);
	signal(SIGQUIT, &spnHandleSignal);
	signal(SIGTSTP, &spnHandleSignal);

	// Ignore pipe errors
	signal(SIGPIPE, SIG_IGN);

	return EXIT_SUCCESS;
}

void spnHandleStartTimer(void)
{
	// Start the minor frame timer
	setitimer (ITIMER_REAL, &timer, NULL);
}

void spnHandleHaltTimer(void)
{
	// Halt the minor frame timer
	setitimer (ITIMER_REAL, NULL, NULL);
}

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
