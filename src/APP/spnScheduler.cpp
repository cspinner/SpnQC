/*
 * spnScheduler.c
 *
 *  Created on: Mar 12, 2015
 *      Author: cspinner
 */
#include "spnQC.h"
#include "HAL.h"
#include "OSAL.h"

using namespace std;

#define LED_STATUS_STEP_COUNT 10

static uint32_t spnSenFrameCount = 0;
static uint32_t spnMinorFrameCount = 0;

static OSAL_Time_Type tsSenStart;
static OSAL_Time_Type tsSenElapsed;
static OSAL_Time_Type tsFgStart;
static OSAL_Time_Type tsFgElapsed;
static OSAL_Time_Type tsSenS2SElapsed;
static OSAL_Time_Type tsIntElapsed;
static OSAL_Time_Type tsSenElapsedMax = {0};
static OSAL_Time_Type tsFgElapsedMax = {0};
static OSAL_Time_Type tsSenS2SElapsedMax = {0};
static OSAL_Time_Type tsIntElapsedMax = {0};
static uint32_t pinLevelPosition = 0;

const uint32_t LED_STATUS_PATTERN[MODE_COUNT_E][LED_STATUS_STEP_COUNT] =
{
	//MODE_INIT_E:
	{1,0,0,0,0,0,0,0,0,0},

	//MODE_ESTABLISH_COMM_E:
	{1,1,0,0,1,1,0,0,1,1},

	//MODE_STANDBY_E:
	{1,0,0,0,0,1,0,0,0,0},

	//MODE_RUN_E:
	{1,1,1,1,1,0,0,0,0,0},

	//MODE_STOP_E:
	{1,1,1,1,1,1,1,1,1,1},

	//MODE_CALIBRATE_E:
	{1,0,1,0,1,0,1,0,1,0},

	//MODE_LOST_COMM_E:
	{1,0,1,0,1,0,0,0,0,0},
};

void spnSchedulerPollSensors(void)
{
	static uint32_t spnFrameCountStart = 0;
	static uint32_t sensorAcquireIndex = 0;

	OSAL_Time_Type tsEnd = spnUtilsAddToTimestamp(&tsSenStart, 0, SENSOR_FRAME_TIME_USEC);
	OSAL_Time_Type tsMid;

	// wait for at least SENSOR_FRAME_TIME_USEC to elapse since the last time this function ran
	while(!spnUtilsTimedOut(&tsEnd)) {}

	// get time between frames
	if(spnSenFrameCount > 0)
	{
		// get elapsed time since last start of frame
		tsSenS2SElapsed = spnUtilsGetElapsedTime(&tsSenStart);
		if(spnUtilsTimeCompare(&tsSenS2SElapsed, &tsSenS2SElapsedMax) == true)
		{
			tsSenS2SElapsedMax = tsSenS2SElapsed;
		}
	}

	// Start of frame time stamp
	spnUtilsMarkTimestamp(&tsSenStart);

	//
	// START scheduling routines:
	//

	if(spnFrameCountStart != spnMinorFrameCount)
	{
		sensorAcquireIndex = 0;
		spnFrameCountStart = spnMinorFrameCount;
	}

	spnSensorManagerPollSensors(); // 1st poll
	spnUtilsMarkTimestamp(&tsMid);
	spnUserOutputSensorUpdate(sensorAcquireIndex);
	sensorAcquireIndex++;

	// wait for at least SENSOR_FRAME_TIME_USEC/2 to elapse before sampling again
	tsMid = spnUtilsAddToTimestamp(&tsEnd, 0, SENSOR_FRAME_TIME_USEC/2);
	while(!spnUtilsTimedOut(&tsMid)) {}

	spnSensorManagerPollSensors(); // 2nd poll

	spnUserOutputSensorUpdate(sensorAcquireIndex);
	sensorAcquireIndex++;

	//
	// END scheduling routines
	//

	// get elapsed time since start of frame
	tsSenElapsed = spnUtilsGetElapsedTime(&tsSenStart);
	if(spnUtilsTimeCompare(&tsSenElapsed, &tsSenElapsedMax) == true)
	{
		tsSenElapsedMax = tsSenElapsed;
	}

	spnSenFrameCount++;
}

void spnSchedulerForeground(void)
{
	// get time between frames
	if(spnMinorFrameCount > 0)
	{
		// get elapsed time since last start of frame
		tsIntElapsed = spnUtilsGetElapsedTime(&tsFgStart);
		if(spnUtilsTimeCompare(&tsIntElapsed, &tsIntElapsedMax) == true)
		{
			tsIntElapsedMax = tsIntElapsed;
		}
	}

	// Start of frame time stamp
	spnUtilsMarkTimestamp(&tsFgStart);

	//
	// START scheduling routines:
	//

	spnUserInputUpdate();
	spnModeUpdate();

	// Flash LED - 2 second cycle time for entire LED pattern.
	//             And each pattern has LED_STATUS_STEP_COUNT positions.
	if(spnMinorFrameCount %
			(uint32_t)((2000/(MINOR_FRAME_TIME_USEC/1000))/LED_STATUS_STEP_COUNT) == 0)
	{
		HAL_STATUS_LED_SET(LED_STATUS_PATTERN[spnModeGet()][pinLevelPosition]) ;
		pinLevelPosition = (pinLevelPosition+1) % LED_STATUS_STEP_COUNT;
	}

	spnSensorManagerProcessData();
	spnCommandUpdate();

	// consume the input character
//	printf("mode: %s, totmfcnt: %i, char: %c\n",  spnModeGetString() , spnMinorFrameCount, spnUserInputCharGet(true));
	spnUserInputCharGet(true);

	// Print outputs
	spnUserOutputUpdate(spnMinorFrameCount);

	//
	// END scheduling routines
	//

	// get elapsed time since start of frame
	tsFgElapsed = spnUtilsGetElapsedTime(&tsFgStart);
	if(spnUtilsTimeCompare(&tsFgElapsed, &tsFgElapsedMax) == true)
	{
		tsFgElapsedMax = tsFgElapsed;
	}

	spnMinorFrameCount++;
}

void spnSchedulerGetFrameTime(uint32_t* pSec,
						      uint32_t* pMSec,
							  uint32_t* pUSec)
{
	*pSec = tsFgElapsed.seconds;
	*pMSec = tsFgElapsed.microSeconds/1000;
	*pUSec = tsFgElapsed.microSeconds%1000;
}

void spnSchedulerGetSensorPollTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec)
{
	*pSec = tsSenElapsed.seconds;
	*pMSec = tsSenElapsed.microSeconds/1000;
	*pUSec = tsSenElapsed.microSeconds%1000;
}

void spnSchedulerGetMaxSensorPollTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec)
{
	*pSec = tsSenElapsedMax.seconds;
	*pMSec = tsSenElapsedMax.microSeconds/1000;
	*pUSec = tsSenElapsedMax.microSeconds%1000;
}

void spnSchedulerGetSenStart2StartTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec)
{
	*pSec = tsSenS2SElapsed.seconds;
	*pMSec = tsSenS2SElapsed.microSeconds/1000;
	*pUSec = tsSenS2SElapsed.microSeconds%1000;
}

void spnSchedulerGetMaxSenStart2StartTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec)
{
	*pSec = tsSenS2SElapsedMax.seconds;
	*pMSec = tsSenS2SElapsedMax.microSeconds/1000;
	*pUSec = tsSenS2SElapsedMax.microSeconds%1000;
}

void spnSchedulerGetMaxFrameTime(uint32_t* pSec,
						         uint32_t* pMSec,
							     uint32_t* pUSec)
{
	*pSec = tsFgElapsedMax.seconds;
	*pMSec = tsFgElapsedMax.microSeconds/1000;
	*pUSec = tsFgElapsedMax.microSeconds%1000;
}

void spnSchedulerGetIntTime(uint32_t* pSec,
						      uint32_t* pMSec,
							  uint32_t* pUSec)
{
	*pSec = tsIntElapsed.seconds;
	*pMSec = tsIntElapsed.microSeconds/1000;
	*pUSec = tsIntElapsed.microSeconds%1000;
}

void spnSchedulerGetMaxIntTime(uint32_t* pSec,
						         uint32_t* pMSec,
							     uint32_t* pUSec)
{
	*pSec = tsIntElapsedMax.seconds;
	*pMSec = tsIntElapsedMax.microSeconds/1000;
	*pUSec = tsIntElapsedMax.microSeconds%1000;
}

uint32_t spnSchedulerGetFrameCount(void)
{
	return spnMinorFrameCount;
}


