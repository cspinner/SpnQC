/*
 * spnUserOutput.cpp
 *
 *  Created on: Aug 15, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include <stdlib.h>
#include <stdio.h>

static FILE* pOutputFile = NULL; // output file descriptor
static uint32_t senElapsedSec;
static uint32_t senElapsedMSec;
static uint32_t senElapsedUSec;
static uint32_t senElapsedMaxSec;
static uint32_t senElapsedMaxMSec;
static uint32_t senElapsedMaxUSec;
static uint32_t fgElapsedSec;
static uint32_t fgElapsedMSec;
static uint32_t fgElapsedUSec;
static uint32_t fgElapsedMaxSec;
static uint32_t fgElapsedMaxMSec;
static uint32_t fgElapsedMaxUSec;
static uint32_t senS2SElapsedSec;
static uint32_t senS2SElapsedMSec;
static uint32_t senS2SElapsedUSec;
static uint32_t senS2SElapsedMaxSec;
static uint32_t senS2SElapsedMaxMSec;
static uint32_t senS2SElapsedMaxUSec;
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
static SpnNineAxisMotion_Data_Type nineAxesData;

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
				"SYSMODE,CMDMODE,FRAME,YAW,PITCH,ROLL,TEMP_F,INPW0,MOTCMD0,MOTCMD1,MOTCMD2,MOTCMD3,THROT,ELEV,AILER,RUDD,GYROX,GYROY,GYROZ,ACCX,ACCY,ACCZ,FRAME_TIME_S,FRAME_TIME_MS,FRAME_TIME_US,INT_TIME_S,INT_TIME_MS,INT_TIME_US,SEN_TIME_S,SEN_TIME_MS,SEN_TIME_US,SEN_MAX_TIME_S,SEN_MAX_TIME_MS,SEN_MAX_TIME_US,SEN_S2S_TIME_S,SEN_S2S_TIME_MS,SEN_S2S_TIME_US,SEN_S2S_MAX_TIME_S,SEN_S2S_MAX_TIME_MS,SEN_S2S_MAX_TIME_US\n");
		spnUtilsWriteToFile(pOutputFile,
				",,,DEGREES,DEGREES,DEGREES,DEG_F,USEC,PCT,PCT,PCT,PCT,PCT,DEGREES,DEGREES,DEGREES,DEG/S,DEG/S,DEG/S,G,G,G,SEC,MSEC,USEC,SEC,MSEC,USEC,SEC,MSEC,USEC,SEC,MSEC,USEC,SEC,MSEC,USEC,SEC,MSEC,USEC\n");

		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

void spnUserOutputUpdate(void)
{
	// Update data for output
	spnSchedulerGetFrameTime(&fgElapsedSec, &fgElapsedMSec, &fgElapsedUSec);
	spnSchedulerGetSensorPollTime(&senElapsedSec, &senElapsedMSec, &senElapsedUSec);
	spnSchedulerGetMaxSensorPollTime(&senElapsedMaxSec, &senElapsedMaxMSec, &senElapsedMaxUSec);
	spnSchedulerGetMaxFrameTime(&fgElapsedMaxSec, &fgElapsedMaxMSec, &fgElapsedMaxUSec);
	spnSchedulerGetSenStart2StartTime(&senS2SElapsedSec, &senS2SElapsedMSec, &senS2SElapsedUSec);
	spnSchedulerGetMaxSenStart2StartTime(&senS2SElapsedMaxSec, &senS2SElapsedMaxMSec, &senS2SElapsedMaxUSec);
	spnSchedulerGetIntTime(&intElapsedSec, &intElapsedMSec, &intElapsedUSec);
	spnSchedulerGetMaxIntTime(&intElapsedMaxSec, &intElapsedMaxMSec, &intElapsedMaxUSec);
	spnSensorGetPrincipalAxes(&Pitch, &Roll, &Yaw);
	spnSensorGetNineAxesData(&nineAxesData);
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
	printf("Sensor time: %u sec, %u msec, %u usec\n", senElapsedSec, senElapsedMSec, senElapsedUSec);
	printf("Frame time: %u sec, %u msec, %u usec\n", fgElapsedSec, fgElapsedMSec, fgElapsedUSec);

	printf("\n");

	printf("Temperature (F): %f\n", Temperature);
	printf("%-5s: Yaw: %10f, Pitch: %10f, Roll: %10f\n", "Pos", Yaw, Pitch, Roll);

	printf("\n");
	printf("Input Pulse 0: %u\n", spnServoGetPulseWidth(0));

	for(uint32_t i = 0; i < 4; i++)
	{
		printf("Motor Cmd %i: %.1f\n", i, spnMotorsGet(i));
	}

	printf("\n");
	printf("Throttle Cmd: %.1f %%\n", spnTransceiverGetThrottlePct());
	printf("Elevator Cmd: %.1f degrees\n", spnTransceiverGetElevatorAngle());
	printf("Aileron Cmd: %.1f degrees\n", spnTransceiverGetAileronAngle());
	printf("Rudder Cmd: %.1f degrees\n", spnTransceiverGetRudderAngle());
}

static void userOutputFile(void)
{
	// write data to file
	char buf[1024];
	sprintf(buf, "%s,%s,%i,%.1f,%.1f,%.1f,%.1f,%u,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
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
			nineAxesData.gyro.x,
			nineAxesData.gyro.y,
			nineAxesData.gyro.z,
			nineAxesData.accel.x,
			nineAxesData.accel.y,
			nineAxesData.accel.z,
			fgElapsedSec,
			fgElapsedMSec,
			fgElapsedUSec,
			intElapsedSec,
			intElapsedMSec,
			intElapsedUSec,
			senElapsedSec,
			senElapsedMSec,
			senElapsedUSec,
			senElapsedMaxSec,
			senElapsedMaxMSec,
			senElapsedMaxUSec,
			senS2SElapsedSec,
			senS2SElapsedMSec,
			senS2SElapsedUSec,
			senS2SElapsedMaxSec,
			senS2SElapsedMaxMSec,
			senS2SElapsedMaxUSec);

	spnUtilsWriteToFile(pOutputFile, buf);
}

static void userOutputOnExit(void)
{
	printf("Closing Output File...\n");
	spnUtilsCloseFile(pOutputFile);
}

