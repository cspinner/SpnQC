/*
 * OSAL_TIME.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */
#include "OSAL.h"
#include <Windows.h>
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

static int gettimeofday(struct timeval * tp, struct timezone * tzp);

bool OSAL_TIME_ADD(OSAL_Time_Type* pA, OSAL_Time_Type* pB, OSAL_Time_Type* pResult)
{
	pResult->seconds = pA->seconds + pB->seconds;
	pResult->microSeconds = pA->microSeconds + pB->microSeconds;
	if (pResult->microSeconds >= 1000000)
	{
		++pResult->seconds;
		pResult->microSeconds -= 1000000;
	}

	return EXIT_SUCCESS;
}

bool OSAL_TIME_SUB(OSAL_Time_Type* pA, OSAL_Time_Type* pB, OSAL_Time_Type* pResult)
{
	pResult->seconds = pA->seconds - pB->seconds;
	pResult->microSeconds = pA->microSeconds - pB->microSeconds;

	if (pResult->microSeconds < 0)
	{
		--pResult->seconds;
		pResult->microSeconds += 1000000;
	}

	return EXIT_SUCCESS;
}

bool OSAL_TIME_OF_DAY(OSAL_Time_Type* pTimeOfDay)
{
	struct timeval result;

	gettimeofday(&result, 0);

	TV2OSAL(pTimeOfDay, &result);

	return EXIT_SUCCESS;
}

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}
