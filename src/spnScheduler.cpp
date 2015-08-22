/*
 * spnScheduler.c
 *
 *  Created on: Mar 12, 2015
 *      Author: cspinner
 */
#include "spnQC.h"
#include "wiringPi.h"
#include <time.h>

using namespace std;

#define LED_STATUS_STEP_COUNT 10

static int spnMinorFrameCount = 0;
static struct timeval tsFgElapsed;
static struct timeval tsIntElapsed;
static struct timeval tsFgElapsedMax = {0};
static struct timeval tsIntElapsedMax = {0};
static int pinLevelPosition = 0;

const int LED_STATUS_PATTERN[MODE_COUNT_E][LED_STATUS_STEP_COUNT] =
{
	//MODE_INIT_E:
	{1,0,0,0,0,0,0,0,0,0},

	//MODE_STANDBY_E:
	{1,0,0,0,0,1,0,0,0,0},

	//MODE_RUN_E:
	{1,1,1,1,1,0,0,0,0,0},

	//MODE_STOP_E:
	{1,1,1,1,1,1,1,1,1,1},

	//MODE_CALIBRATE_E:
	{1,0,1,0,1,0,1,0,1,0}
};

void spnSchedulerForeground(void)
{
	// get time between frames
	if(spnMinorFrameCount > 0)
	{
		// get elapsed time since last start of frame
		tsIntElapsed = spnUtilsGetElapsedTime();
		if(spnUtilsTimeCompare(&tsIntElapsed, &tsIntElapsedMax) == true)
		{
			tsIntElapsedMax = tsIntElapsed;
		}
	}

	// Start of frame time stamp
	spnUtilsMarkTimestamp();

	spnUserInputUpdate();
	spnModeUpdate();

	// Flash LED - 2 second cycle time for entire LED pattern.
	//             And each pattern has LED_STATUS_STEP_COUNT positions.
	if(spnMinorFrameCount %
			(int)((2000/(MINOR_FRAME_TIME_USEC/1000))/LED_STATUS_STEP_COUNT) == 0)
	{
		digitalWrite (OUTPUT_PIN_STATUS_LED,
				LED_STATUS_PATTERN[spnModeGet()][pinLevelPosition]) ;
		pinLevelPosition = (pinLevelPosition+1) % LED_STATUS_STEP_COUNT;
	}

	spnSensorManagerUpdate();
	spnCommandUpdate();

	// consume the input character
	spnUserInputCharGet(true);

	// get elapsed time since start of frame
	tsFgElapsed = spnUtilsGetElapsedTime();
	if(spnUtilsTimeCompare(&tsFgElapsed, &tsFgElapsedMax) == true)
	{
		tsFgElapsedMax = tsFgElapsed;
	}

	// Print outputs
	spnUserOutputUpdate();

	spnMinorFrameCount++;
}

void spnSchedulerGetFrameTime(unsigned int* pSec,
						      unsigned int* pMSec,
							  unsigned int* pUSec)
{
	*pSec = tsFgElapsed.tv_sec;
	*pMSec = tsFgElapsed.tv_usec/1000;
	*pUSec = tsFgElapsed.tv_usec%1000;
}

void spnSchedulerGetMaxFrameTime(unsigned int* pSec,
						         unsigned int* pMSec,
							     unsigned int* pUSec)
{
	*pSec = tsFgElapsedMax.tv_sec;
	*pMSec = tsFgElapsedMax.tv_usec/1000;
	*pUSec = tsFgElapsedMax.tv_usec%1000;
}

void spnSchedulerGetIntTime(unsigned int* pSec,
						      unsigned int* pMSec,
							  unsigned int* pUSec)
{
	*pSec = tsIntElapsed.tv_sec;
	*pMSec = tsIntElapsed.tv_usec/1000;
	*pUSec = tsIntElapsed.tv_usec%1000;
}

void spnSchedulerGetMaxIntTime(unsigned int* pSec,
						         unsigned int* pMSec,
							     unsigned int* pUSec)
{
	*pSec = tsIntElapsedMax.tv_sec;
	*pMSec = tsIntElapsedMax.tv_usec/1000;
	*pUSec = tsIntElapsedMax.tv_usec%1000;
}

int spnSchedulerGetFrameCount(void)
{
	return spnMinorFrameCount;
}


