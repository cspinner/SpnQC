/*
 * spnQC.h
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#ifndef SPNQC_H_
#define SPNQC_H_

#include "spnConfig.h"
#include "OSAL.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

//
// DEFINES
//
#ifdef TRACEPOINT
#define PRINT(S) printf("%s\n", S);
#else
#define PRINT(S)
#endif

//#define NO_FILTER 1
#define SENSOR_FRAME_TIME_USEC 5000
#define MINOR_FRAME_TIME_USEC 30000

#define PI 3.1415926538

#define min(a,b) \
   ({ typeof (a) _a = (a); \
      typeof (b) _b = (b); \
     _a < _b ? _a : _b; })

#define max(a,b) \
   ({ typeof (a) _a = (a); \
      typeof (b) _b = (b); \
     _a > _b ? _a : _b; })

/**
 * clamp - return a value clamped to a given range with strict typechecking
 * @val: current value
 * @lo: lowest allowable value
 * @hi: highest allowable value
 *
 * This macro does strict typechecking of lo/hi to make sure they are of the
 *  same type as val.  See the unnecessary pointer comparisons.
 *
 * From kernel.h
 */
#define clamp(val, lo, hi) min((typeof(val))max(val, lo), hi)


//
// TYPES
//
typedef enum
{
	MODE_INIT_E,
	MODE_ESTABLISH_COMM_E,
	MODE_STANDBY_E,
	MODE_RUN_E,
	MODE_STOP_E,
	MODE_CALIBRATE_E,
	MODE_LOST_COMM_E,
	MODE_COUNT_E
} System_Mode_Type;

typedef float  float32_t;
typedef double float64_t;

//
// DATA
//

//
// METHODS
//
bool spnInit(void);
bool spnInitCompleted(void);

bool spnHandleInit(void);
void spnHandleStartTimer(void);
void spnHandleHaltTimer(void);

void spnSchedulerForeground(void);
void spnSchedulerPollSensors(void);
void spnSchedulerGetFrameTime(uint32_t* pSec,
						      uint32_t* pMSec,
							  uint32_t* pUSec);
void spnSchedulerGetSensorPollTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec);
void spnSchedulerGetMaxSensorPollTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec);
void spnSchedulerGetSenStart2StartTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec);
void spnSchedulerGetMaxSenStart2StartTime(uint32_t* pSec,
								   uint32_t* pMSec,
								   uint32_t* pUSec);
void spnSchedulerGetMaxFrameTime(uint32_t* pSec,
						         uint32_t* pMSec,
							     uint32_t* pUSec);
void spnSchedulerGetIntTime(uint32_t* pSec,
						      uint32_t* pMSec,
							  uint32_t* pUSec);
void spnSchedulerGetMaxIntTime(uint32_t* pSec,
						         uint32_t* pMSec,
							     uint32_t* pUSec);
uint32_t spnSchedulerGetFrameCount(void);

bool spnSensorManagerInit(void);
void spnSensorManagerProcessData(void);
void spnSensorManagerPollSensors(void);
void spnSensorGetPrincipalAxes(float32_t* pPitch, float32_t* pRoll, float32_t* pYaw);
void spnSensorGetNineAxesData(float32_t* accel, float32_t* gyro, float32_t* mag);
void spnSensorGetRawNineAxesData(float32_t* accel, float32_t* gyro, float32_t* mag, uint32_t frame);
void spnSensorGetTemperature(float32_t* temperature);

bool spnCommandInit(void);
void spnCommandUpdate(void);
const char* spnCommandGetModeString(void);

const char* spnModeGetString(void);
System_Mode_Type spnModeGet(void);
void spnModeUpdate(void);

bool spnUtilsTimedOut(OSAL_Time_Type* pTsEnd);
void spnUtilsWaitUsec(uint32_t delayUsec);
OSAL_Time_Type spnUtilsAddToTimestamp(OSAL_Time_Type* pTimeStamp, uint32_t sec, uint32_t usec);
void spnUtilsMarkTimestamp(OSAL_Time_Type* pTimeStamp);
OSAL_Time_Type spnUtilsGetElapsedTime(OSAL_Time_Type* pTimeStamp);
bool spnUtilsTimeCompare(OSAL_Time_Type* pTsA, OSAL_Time_Type* pTsB);
bool spnUtilsOpenFileForRead(uint32_t* pFileId, const char *pPathname);
uint32_t spnUtilsReadLine(uint32_t fileId, char* pDest, size_t destSizeBytes);
bool spnUtilsReadNextFloatFromFile(uint32_t fileId, float32_t* pDest);
bool spnUtilsReadNextIntFromFile(uint32_t fileId, int32_t* pDest);
bool spnUtilsOpenFileForAppend(uint32_t* pFileId, const char *pPathname);
bool spnUtilsCreateFileForWrite(uint32_t* pFileId, const char *pPathname);
void spnUtilsWriteToFile(uint32_t fileId, const char *pBuf);
void spnUtilsCloseInputFile(uint32_t fileId);
void spnUtilsCloseOutputFile(uint32_t fileId);

bool spnUserOutputInit(void);
void spnUserOutputUpdate(uint32_t frame);
void spnUserOutputSensorUpdate(uint32_t frame);

bool spnUserInputInit(void);
void spnUserInputUpdate(void);
char spnUserInputCharGet(bool consume);
bool spnUserInputCheckHeartbeat(void);
bool spnUserInputCommEstablished(void);

bool spnMotorsInit(void);
void spnMotorsSet(uint32_t motorNum, float32_t cmdPct);
float32_t spnMotorsGet(uint32_t motorNum);
void spnMotorsCalibrateDrive(uint32_t level);
void spnMotorsStopAll(void);

bool spnTransceiverInit(void);
float32_t spnTransceiverGetThrottlePct(void);
float32_t spnTransceiverGetElevatorAngle(void);
float32_t spnTransceiverGetAileronAngle(void);
float32_t spnTransceiverGetRudderAngle(void);
bool spnTransceiverIsEnableSet(void);
bool spnTransceiverIsActive(void);

bool spnServerInit(uint16_t port);
void spnServerWaitForGroundStation(void);
uint32_t spnServerReadMessage(char* buf, uint32_t maxReadBytes);
void spnServerWriteMessage(char* buf, uint32_t sizeBytes);

#endif /* SPNQC_H_ */
