/*
 * spnUserOutput.cpp
 *
 *  Created on: Aug 15, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"
#include <stdlib.h>
#include <stdio.h>

static FILE* pOutputFile = NULL; // output file descriptor
static unsigned int fgElapsedSec;
static unsigned int fgElapsedMSec;
static unsigned int fgElapsedUSec;
static unsigned int fgElapsedMaxSec;
static unsigned int fgElapsedMaxMSec;
static unsigned int fgElapsedMaxUSec;
static unsigned int intElapsedSec;
static unsigned int intElapsedMSec;
static unsigned int intElapsedUSec;
static unsigned int intElapsedMaxSec;
static unsigned int intElapsedMaxMSec;
static unsigned int intElapsedMaxUSec;
static float Pitch;
static float Roll;
static float Yaw;
static float Temperature;

static void userOutputOnExit(void);
static void userOutputConsole(void);
static void userOutputFile(void);

bool spnUserOutputInit(void)
{
	// open the file
	spnUtilsCreateFileForWrite(&pOutputFile, "outputData.csv");

	if(pOutputFile != NULL)
	{
		atexit(&userOutputOnExit);

		// write all headings
		spnUtilsWriteToFile(pOutputFile,
				"SYSMODE,CMDMODE,FRAME,YAW,PITCH,ROLL,TEMP_F,FRAME_TIME_S,FRAME_TIME_MS,FRAME_TIME_US,INT_TIME_S,INT_TIME_MS,INT_TIME_US\n");

		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

void spnUserOutputUpdate(void)
{
	// Update data for output
	spnSchedulerGetFrameTime(&fgElapsedSec, &fgElapsedMSec, &fgElapsedUSec);
	spnSchedulerGetMaxFrameTime(&fgElapsedMaxSec, &fgElapsedMaxMSec, &fgElapsedMaxUSec);
	spnSchedulerGetIntTime(&intElapsedSec, &intElapsedMSec, &intElapsedUSec);
	spnSchedulerGetMaxIntTime(&intElapsedMaxSec, &intElapsedMaxMSec, &intElapsedMaxUSec);
	spnSensorGetPrincipalAxes(&Pitch, &Roll, &Yaw);
	Temperature = spnSensorGetTemperature();

	// Output the data
	userOutputConsole();
	userOutputFile();
}

static void userOutputConsole(void)
{
	// Clear screen
	for(int i = 0; i < 200; i++) printf("\n");

	printf("System Mode: %s - Command Mode: %s\n", spnModeGetString(), spnCommandGetModeString());
	printf("Frame count: %i\n", spnSchedulerGetFrameCount());
	printf("Frame time: %u sec, %u msec, %u usec\n", fgElapsedSec, fgElapsedMSec, fgElapsedUSec);
	printf("Max Frame time: %u sec, %u msec, %u usec\n", fgElapsedMaxSec, fgElapsedMaxMSec, fgElapsedMaxUSec);
	printf("Interrupt time: %u sec, %u msec, %u usec\n", intElapsedSec, intElapsedMSec, intElapsedUSec);
	printf("Max Interrupt time: %u sec, %u msec, %u usec\n", intElapsedMaxSec, intElapsedMaxMSec, intElapsedMaxUSec);

	printf("\n");

	printf("Temperature (F): %f\n", Temperature);
	printf("%-5s: Yaw: %10f, Pitch: %10f, Roll: %10f\n", "Pos", Yaw, Pitch, Roll);
}

static void userOutputFile(void)
{
	// write data to file
	char buf[1024];
	sprintf(buf, "%s,%s,%i,%.5f,%.5f,%.5f,%.5f,%u,%u,%u,%u,%u,%u\n",
			spnModeGetString(),
			spnCommandGetModeString(),
			spnSchedulerGetFrameCount(),
			Yaw,
			Pitch,
			Roll,
			Temperature,
			fgElapsedSec,
			fgElapsedMSec,
			fgElapsedUSec,
			intElapsedSec,
			intElapsedMSec,
			intElapsedUSec);

	spnUtilsWriteToFile(pOutputFile, buf);
}

static void userOutputOnExit(void)
{
	printf("Closing Output File...\n");
	spnUtilsCloseFile(pOutputFile);
}

