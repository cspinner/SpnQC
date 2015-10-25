/*
 * OSAL_TIME.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */

#include "OSAL.h"
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>

#define OSAL2TV(d,s)                     \
do                                        \
{                                         \
	(d)->tv_sec = (s)->seconds;           \
	(d)->tv_usec = (s)->microSeconds;     \
} while (0)

#define TV2OSAL(d,s)                     \
do                                        \
{                                         \
	(d)->seconds = (s)->tv_sec;           \
	(d)->microSeconds = (s)->tv_usec;     \
} while (0)

static struct itimerval timer = { 0 };

bool OSAL_TIME_ADD(OSAL_Time_Type* pA, OSAL_Time_Type* pB, OSAL_Time_Type* pResult)
{
	struct timeval a, b, result;

	OSAL2TV(&a, pA);
	OSAL2TV(&b, pB);

	timeradd(&a, &b, &result);

	TV2OSAL(pResult, &result);

	return EXIT_SUCCESS;
}

bool OSAL_TIME_SUB(OSAL_Time_Type* pA, OSAL_Time_Type* pB, OSAL_Time_Type* pResult)
{
	struct timeval a, b, result;

	OSAL2TV(&a, pA);
	OSAL2TV(&b, pB);

	timersub(&a, &b, &result);

	TV2OSAL(pResult, &result);

	return EXIT_SUCCESS;
}

bool OSAL_TIME_OF_DAY(OSAL_Time_Type* pTimeOfDay)
{
	struct timeval result;

	gettimeofday(&result, 0);

	TV2OSAL(pTimeOfDay, &result);

	return EXIT_SUCCESS;
}

bool OSAL_TIME_INT_SET(uint32_t intervalUsec)
{
	// Set the interval timer
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = intervalUsec;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = intervalUsec;

	return EXIT_SUCCESS;
}

bool OSAL_TIME_INT_START(void)
{
	setitimer (ITIMER_REAL, &timer, NULL);

	return EXIT_SUCCESS;
}

bool OSAL_TIME_INT_STOP(void)
{
	setitimer (ITIMER_REAL, NULL, NULL);

	return EXIT_SUCCESS;
}
