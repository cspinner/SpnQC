/*
 * spnQC.h
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#ifndef SPNQC_H_
#define SPNQC_H_

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
#define INPUT_PIN_TEST_MODE 17

#define PI 3.1415926538

//
// TYPES
//
typedef enum
{
	MODE_INIT_E,
	MODE_TEST_E,
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

bool spnMotorManagerInit(void);
void spnMotorManagerUpdate(void);

bool spnStatusGet(void);
void spnStatusSet(bool);

const char* spnModeGetString(void);
System_Mode_Type spnModeGet(void);
void spnModeSet(System_Mode_Type mode);

void spnUtilsWaitUsec(unsigned int delayUsec);
void spnUtilsMarkTimestamp(void);
struct timeval spnUtilsGetElapsedTime(void);
bool spnUtilsTimeCompare(struct timeval* pTsA, struct timeval* pTsB);
void spnUtilsOpenFileForRead(FILE **pFile, const char *pPathname);
size_t spnUtilsReadLine(FILE *pFile, char* pDest, size_t destSizeBytes);
void spnUtilsOpenFileForAppend(FILE **pFile, const char *pPathname);
void spnUtilsCreateFileForWrite(FILE **pFile, const char *pPathname);
void spnUtilsWriteToFile(FILE *pFile, const char *pBuf);
void spnUtilsCloseFile(FILE *pFile);

bool spnUserOutputInit(void);
void spnUserOutputUpdate(void);






#endif /* SPNQC_H_ */
