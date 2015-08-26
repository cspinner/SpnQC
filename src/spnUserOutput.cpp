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
static uint32_t fgElapsedSec;
static uint32_t fgElapsedMSec;
static uint32_t fgElapsedUSec;
static uint32_t fgElapsedMaxSec;
static uint32_t fgElapsedMaxMSec;
static uint32_t fgElapsedMaxUSec;
static uint32_t intElapsedSec;
static uint32_t intElapsedMSec;
static uint32_t intElapsedUSec;
static uint32_t intElapsedMaxSec;
static uint32_t intElapsedMaxMSec;
static uint32_t intElapsedMaxUSec;
static float32_t Pitch;
static float32_t Roll;
static float32_t Yaw;
static float32_t Temperature;

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

		// write all headings and units
		spnUtilsWriteToFile(pOutputFile,
				"SYSMODE,CMDMODE,FRAME,YAW,PITCH,ROLL,TEMP_F,INPW0,MOTCMD0,MOTCMD1,MOTCMD2,MOTCMD3,THROT,ELEV,AILER,RUDD,FRAME_TIME_S,FRAME_TIME_MS,FRAME_TIME_US,INT_TIME_S,INT_TIME_MS,INT_TIME_US\n");
		spnUtilsWriteToFile(pOutputFile,
				",,,DEGREES,DEGREES,DEGREES,DEG_F,USEC,PCT,PCT,PCT,PCT,PCT,DEGREES,DEGREES,DEGREES,SEC,MSEC,USEC,SEC,MSEC,USEC,\n");

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
	for(uint32_t i = 0; i < 200; i++) printf("\n");

	printf("System Mode: %s - Command Mode: %s\n", spnModeGetString(), spnCommandGetModeString());
	printf("Frame count: %i\n", spnSchedulerGetFrameCount());
	printf("Frame time: %u sec, %u msec, %u usec\n", fgElapsedSec, fgElapsedMSec, fgElapsedUSec);
	printf("Max Frame time: %u sec, %u msec, %u usec\n", fgElapsedMaxSec, fgElapsedMaxMSec, fgElapsedMaxUSec);
	printf("Interrupt time: %u sec, %u msec, %u usec\n", intElapsedSec, intElapsedMSec, intElapsedUSec);
	printf("Max Interrupt time: %u sec, %u msec, %u usec\n", intElapsedMaxSec, intElapsedMaxMSec, intElapsedMaxUSec);

	printf("\n");

	printf("Temperature (F): %f\n", Temperature);
	printf("%-5s: Yaw: %10f, Pitch: %10f, Roll: %10f\n", "Pos", Yaw, Pitch, Roll);
	printf("Input Pulse 0: %u\n", spnServoGetPulseWidth(0));

	for(uint32_t i = 0; i < 4; i++)
	{
		printf("Motor Cmd %i: %.1f\n", i, spnMotorsGet(i));
	}
}

static void userOutputFile(void)
{
	// write data to file
	uint8_t buf[1024];
	sprintf(buf, "%s,%s,%i,%.1f,%.1f,%.1f,%.1f,%u,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%u,%u,%u,%u,%u,%u\n",
			spnModeGetString(),
			spnCommandGetModeString(),
			spnSchedulerGetFrameCount(),
			Yaw,
			Pitch,
			Roll,
			Temperature,
			spnServoGetPulseWidth(0),
			spnMotorsGet(0),
			spnMotorsGet(1),
			spnMotorsGet(2),
			spnMotorsGet(3),
			spnTransceiverGetThrottlePct(),
			spnTransceiverGetElevatorAngle(),
			spnTransceiverGetAileronAngle(),
			spnTransceiverGetRudderAngle(),
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

