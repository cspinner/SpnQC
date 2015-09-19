/*
 * spnQC.h
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#ifndef SPNQC_H_
#define SPNQC_H_

#include "spnConfig.h"
#include "spnNineAxisMotion.h"
#include <time.h>
#include <stdio.h>
#include <stddef.h>
#include <signal.h>
#include <stdint.h>

//
// DEFINES
//
//#define NO_FILTER 1
#define SENSOR_FRAME_TIME_USEC 5000
#define MINOR_FRAME_TIME_USEC 30000

#define OUTPUT_PIN_STATUS_LED 27

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
void spnHandleRegisterCallback(__sighandler_t pCallback);
void spnHandleSignal(int32_t);

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
void spnSensorManagerUpdate(void);
void spnSensorManagerPollSensors(void);
void spnSensorGetPrincipalAxes(float32_t* pPitch, float32_t* pRoll, float32_t* pYaw);
void spnSensorGetRawNineAxesData(SpnNineAxisMotion_Data_Type* pSensorData, uint32_t index);
void spnSensorGetNineAxesData(SpnNineAxisMotion_Data_Type* pSensorData);
float32_t spnSensorGetTemperature(void);

bool spnCommandInit(void);
void spnCommandUpdate(void);
const char* spnCommandGetModeString(void);

const char* spnModeGetString(void);
System_Mode_Type spnModeGet(void);
void spnModeUpdate(void);

bool spnUtilsTimedOut(struct timeval* pTsEnd);
void spnUtilsWaitUsec(uint32_t delayUsec);
struct timeval spnUtilsAddToTimestamp(struct timeval* pTimeStamp, __time_t sec, __suseconds_t usec);
void spnUtilsMarkTimestamp(struct timeval* pTimeStamp);
struct timeval spnUtilsGetElapsedTime(struct timeval* pTimeStamp);
bool spnUtilsTimeCompare(struct timeval* pTsA, struct timeval* pTsB);
void spnUtilsOpenFileForRead(FILE **pFile, const char *pPathname);
size_t spnUtilsReadLine(FILE *pFile, char* pDest, size_t destSizeBytes);
bool spnUtilsReadNextFloatFromFile(FILE* pFile, float32_t* pDest);
bool spnUtilsReadNextIntFromFile(FILE* pFile, int32_t* pDest);
void spnUtilsOpenFileForAppend(FILE **pFile, const char *pPathname);
void spnUtilsCreateFileForWrite(FILE **pFile, const char *pPathname);
void spnUtilsWriteToFile(FILE *pFile, const char *pBuf);
void spnUtilsCloseFile(FILE *pFile);

bool spnUserOutputInit(void);
void spnUserOutputUpdate(uint32_t frame);
void spnUserOutputSensorUpdate(uint32_t frame);

bool spnUserInputInit(void);
void spnUserInputUpdate(void);
char spnUserInputCharGet(bool consume);
bool spnUserInputCheckHeartbeat(void);

bool spnMotorsInit(void);
void spnMotorsSet(uint32_t motorNum, float32_t cmdPct);
float32_t spnMotorsGet(uint32_t motorNum);
void spnMotorsCalibrateDrive(uint32_t level);
void spnMotorsStopAll(void);

bool spnServoInit(uint32_t inputCount, const uint32_t* gpioInputList, uint32_t outputCount, const uint32_t* gpioOutputList);
uint32_t spnServoGetPulseWidth(uint32_t gpioIndex);
bool spnServoSetPulseWidth(uint32_t gpioIndex, uint32_t pulseWidthUsec);
uint32_t spnServoGetCommandedPulseWidth(uint32_t gpioIndex);
bool spnServoStopAllPulses(void);

bool spnTransceiverInit(void);
float32_t spnTransceiverGetThrottlePct(void);
float32_t spnTransceiverGetElevatorAngle(void);
float32_t spnTransceiverGetAileronAngle(void);
float32_t spnTransceiverGetRudderAngle(void);

bool spnServerInit(uint16_t port);
void spnServerWaitForGroundStation(void);
uint32_t spnServerReadMessage(char* buf, uint32_t maxReadBytes);
void spnServerWriteMessage(char* buf, uint32_t sizeBytes);

#endif /* SPNQC_H_ */
