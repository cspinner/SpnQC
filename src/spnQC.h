/*
 * spnQC.h
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#ifndef SPNQC_H_
#define SPNQC_H_

#include "SpnConfig.h"
#include <time.h>
#include <stdio.h>
#include <stddef.h>
#include <signal.h>

//
// DEFINES
//
#define SUCCESS true
#define FAIL false
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
	MODE_COUNT_E
} System_Mode_Type;


//
// DATA
//

//
// METHODS
//
bool spnInit(void);
void spnHandleRegisterCallback(__sighandler_t pCallback);
void spnHandleSignal(int);

void spnSchedulerForeground(void);
void spnSchedulerGetFrameTime(unsigned int* pSec,
						      unsigned int* pMSec,
							  unsigned int* pUSec);
void spnSchedulerGetMaxFrameTime(unsigned int* pSec,
						         unsigned int* pMSec,
							     unsigned int* pUSec);
void spnSchedulerGetIntTime(unsigned int* pSec,
						      unsigned int* pMSec,
							  unsigned int* pUSec);
void spnSchedulerGetMaxIntTime(unsigned int* pSec,
						         unsigned int* pMSec,
							     unsigned int* pUSec);
int spnSchedulerGetFrameCount(void);

bool spnSensorManagerInit(void);
void spnSensorManagerUpdate(void);
void spnSensorGetPrincipalAxes(float* pPitch, float* pRoll, float* pYaw);
float spnSensorGetTemperature(void);

bool spnCommandInit(void);
void spnCommandUpdate(void);
const char* spnCommandGetModeString(void);

bool spnStatusGet(void);
void spnStatusSet(bool);

const char* spnModeGetString(void);
System_Mode_Type spnModeGet(void);
void spnModeUpdate(void);

void spnUtilsWaitUsec(unsigned int delayUsec);
void spnUtilsMarkTimestamp(void);
struct timeval spnUtilsGetElapsedTime(void);
bool spnUtilsTimeCompare(struct timeval* pTsA, struct timeval* pTsB);
void spnUtilsOpenFileForRead(FILE **pFile, const char *pPathname);
size_t spnUtilsReadLine(FILE *pFile, char* pDest, size_t destSizeBytes);
bool spnUtilsReadNextFloatFromFile(FILE* pFile, float* pDest);
bool spnUtilsReadNextIntFromFile(FILE* pFile, int* pDest);
void spnUtilsOpenFileForAppend(FILE **pFile, const char *pPathname);
void spnUtilsCreateFileForWrite(FILE **pFile, const char *pPathname);
void spnUtilsWriteToFile(FILE *pFile, const char *pBuf);
void spnUtilsCloseFile(FILE *pFile);

bool spnUserOutputInit(void);
void spnUserOutputUpdate(void);

bool spnUserInputInit(void);
void spnUserInputUpdate(void);
char spnUserInputCharGet(bool consume);

bool spnMotorsInit(void);
void spnMotorsSet(int motorNum, float cmdPct);
float spnMotorsGet(int motorNum);
void spnMotorsCalibrateDrive(int level);
void spnMotorsStopAll(void);

bool spnServoInit(const int* gpioInputList, const int* gpioOutputList);
unsigned int spnServoGetPulseWidth(int gpioIndex);
bool spnServoSetPulseWidth(int gpioIndex, int pulseWidthUsec);
unsigned int spnServoGetCommandedPulseWidth(int gpioIndex);
bool spnServoStopAllPulses(void);

bool spnTransceiverInit(void);
float spnTransceiverGetThrottlePct(void);
float spnTransceiverGetElevatorAngle(void);
float spnTransceiverGetAileronAngle(void);
float spnTransceiverGetRudderAngle(void);

#endif /* SPNQC_H_ */
