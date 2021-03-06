/*
 * spnUserOutput.cpp
 *
 *  Created on: Aug 15, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "HAL.h"
#include "OSAL.h"
#include "spnConfig.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static uint32_t OutputFile; // output file descriptor
static uint32_t OutputSensorFile; // output file descriptor
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
static float32_t AccelInRaw[NUM_AXIS], GyroInRaw[NUM_AXIS], MagInRaw[NUM_AXIS];
static float32_t AccelIn[NUM_AXIS], GyroIn[NUM_AXIS], MagIn[NUM_AXIS];
static bool networkOutputEnabled;

static void userOutputOnExit(void);
static void userOutputConsole(void);
static void userOutputFile(void);
static void userOutputSensorDataFile(void);

bool spnUserOutputInit(void)
{
	PRINT("start spnUserOutputInit");

	const SpnQC_Config_Type* const pCfg = spnConfigGet();

	networkOutputEnabled = pCfg->transceiver.outputToNetwork;

	// open the files
	if((spnUtilsCreateFileForWrite(&OutputFile, "outputData.csv") == EXIT_SUCCESS) &&
	   (spnUtilsCreateFileForWrite(&OutputSensorFile, "outputSensorData.csv") == EXIT_SUCCESS))
	{
		atexit(&userOutputOnExit);

		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

void spnUserOutputUpdate(uint32_t frame)
{
	// Update data for output
	spnSchedulerGetFrameTime(&fgElapsedSec, &fgElapsedMSec, &fgElapsedUSec);
	spnSchedulerGetMaxFrameTime(&fgElapsedMaxSec, &fgElapsedMaxMSec, &fgElapsedMaxUSec);
	spnSchedulerGetIntTime(&intElapsedSec, &intElapsedMSec, &intElapsedUSec);
	spnSchedulerGetMaxIntTime(&intElapsedMaxSec, &intElapsedMaxMSec, &intElapsedMaxUSec);
	spnSensorGetPrincipalAxes(&Pitch, &Roll, &Yaw);
	spnSensorGetNineAxesData(AccelIn, GyroIn, MagIn);
	spnSensorGetTemperature(&Temperature);

	// Output the data
	if((frame % 2) == 0)
	{
		userOutputConsole();
	}

	userOutputFile();
}

void spnUserOutputSensorUpdate(uint32_t frame)
{
	spnSensorGetPrincipalAxes(&Pitch, &Roll, &Yaw);
	spnSensorGetRawNineAxesData(AccelInRaw, GyroInRaw, MagInRaw, frame);
	spnSchedulerGetSensorPollTime(&senElapsedSec, &senElapsedMSec, &senElapsedUSec);
	spnSchedulerGetMaxSensorPollTime(&senElapsedMaxSec, &senElapsedMaxMSec, &senElapsedMaxUSec);
	spnSchedulerGetSenStart2StartTime(&senS2SElapsedSec, &senS2SElapsedMSec, &senS2SElapsedUSec);
	spnSchedulerGetMaxSenStart2StartTime(&senS2SElapsedMaxSec, &senS2SElapsedMaxMSec, &senS2SElapsedMaxUSec);

	userOutputSensorDataFile();
}

static void userOutputConsole(void)
{
	char buffer[65536];
	char tempBuf[1024];

	memset(buffer, 0, sizeof(buffer));

	sprintf(buffer, "BEGINCONOUT");

	// Clear screen
	for(uint32_t i = 0; i < 200; i++) { sprintf(tempBuf, "\n"); strcat(buffer, tempBuf); }

	sprintf(tempBuf, "System Mode: %s - Command Mode: %s\n", spnModeGetString(), spnCommandGetModeString()); strcat(buffer, tempBuf);
	sprintf(tempBuf, "Frame count: %i\n", spnSchedulerGetFrameCount()); strcat(buffer, tempBuf);
	sprintf(tempBuf, "Sensor time: %u sec, %u msec, %u usec\n", senElapsedSec, senElapsedMSec, senElapsedUSec); strcat(buffer, tempBuf);
	sprintf(tempBuf, "Frame time: %u sec, %u msec, %u usec\n", fgElapsedSec, fgElapsedMSec, fgElapsedUSec); strcat(buffer, tempBuf);

	sprintf(tempBuf, "\n"); strcat(buffer, tempBuf);

	sprintf(tempBuf, "Temperature (F): %f\n", Temperature); strcat(buffer, tempBuf);
	sprintf(tempBuf, "%-5s: Yaw: %10f, Pitch: %10f, Roll: %10f\n", "Pos", Yaw, Pitch, Roll); strcat(buffer, tempBuf);
//	sprintf(tempBuf, "%-6.3f, %-6.3f, %-6.3f - %-6.3f, %-6.3f, %-6.3f\n",
//			filtNineAxesData.gyro.x,
//			filtNineAxesData.gyro.y,
//			filtNineAxesData.gyro.z,
//			filtNineAxesData.accel.x,
//			filtNineAxesData.accel.y,
//			filtNineAxesData.accel.z);  strcat(buffer, tempBuf);

	sprintf(tempBuf, "\n"); strcat(buffer, tempBuf);
//	sprintf(tempBuf, "Input Pulse 0: %u\n", HAL_SERVO_PULSE_WIDTH_GET(0)); strcat(buffer, tempBuf);

	for(uint32_t i = 0; i < 4; i++)
	{
		sprintf(tempBuf, "Motor Cmd %i: %.1f\n", i, spnMotorsGet(i)); strcat(buffer, tempBuf);
	}

	sprintf(tempBuf, "\n"); strcat(buffer, tempBuf);
	sprintf(tempBuf, "Throttle Cmd: %.1f %%\n", spnTransceiverGetThrottlePct()); strcat(buffer, tempBuf);
	sprintf(tempBuf, "Elevator Cmd: %.1f degrees\n", spnTransceiverGetElevatorAngle()); strcat(buffer, tempBuf);
	sprintf(tempBuf, "Aileron Cmd: %.1f degrees\n", spnTransceiverGetAileronAngle()); strcat(buffer, tempBuf);
	sprintf(tempBuf, "Rudder Cmd: %.1f degrees\n", spnTransceiverGetRudderAngle()); strcat(buffer, tempBuf);

	// Finally, print the data
	printf("%s", buffer);

	// Send to network if configured
	if(networkOutputEnabled)
	{
		spnServerWriteMessage(buffer, strlen(buffer));
	}

//	float32_t kp, ki, kd;
//	float32_t pterm, iterm, dterm, sumTotal;
//	rollAnglePID.getGains(&kp, &ki, &kd);
//	rollAnglePID.getTerms(&pterm, &iterm, &dterm, &sumTotal);
//	printf("KP=%.3f\n", kp);
//	printf("KI=%.3f\n", ki);
//	printf("KD=%.3f\n\n", kd);
//
//	printf("P=%.5f\n", kp*pterm);
//	printf("I=%.5f\n", ki*iterm);
//	printf("D=%.5f\n", kd*dterm);
//	printf("T=%.5f\n\n", sumTotal);
}

static void userOutputFile(void)
{
	static bool isFirstOutput = true;
	char tempBuf[1024];

	if(isFirstOutput)
	{
		// write all headings and units
		sprintf(tempBuf, "SYSMODE,CMDMODE,FRAME,YAW,PITCH,ROLL,TEMP_F,MOTCMD0,MOTCMD1,MOTCMD2,MOTCMD3,THROT,ELEV,AILER,RUDD,FRAME_TIME_S,FRAME_TIME_MS,FRAME_TIME_US,INT_TIME_S,INT_TIME_MS,INT_TIME_US,GYROX,GYROY,GYROZ,ACCELX,ACCELY,ACCELZ\n");
		spnUtilsWriteToFile(OutputFile, tempBuf);

		sprintf(tempBuf, ",,,DEGREES,DEGREES,DEGREES,DEG_F,PCT,PCT,PCT,PCT,PCT,DEGREES,DEGREES,DEGREES,SEC,MSEC,USEC,SEC,MSEC,USEC,DEG/S,DEG/S,DEG/S,G,G,G\n");
		spnUtilsWriteToFile(OutputFile, tempBuf);

		isFirstOutput = false;
	}

	// write data to file
	sprintf(tempBuf, "%s,%s,%i,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%u,%u,%u,%u,%u,%u,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",
			spnModeGetString(),
			spnCommandGetModeString(),
			spnSchedulerGetFrameCount(),
			Yaw,
			Pitch,
			Roll,
			Temperature,
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
			intElapsedUSec,
			GyroIn[X_AXIS],
			GyroIn[Y_AXIS],
			GyroIn[Z_AXIS],
			AccelIn[X_AXIS],
			AccelIn[Y_AXIS],
			AccelIn[Z_AXIS]);

	spnUtilsWriteToFile(OutputFile, tempBuf);
}

static void userOutputSensorDataFile(void)
{
	static bool isFirstOutput = true;

	if(isFirstOutput)
	{
		// write all headings and units
		spnUtilsWriteToFile(OutputSensorFile,
				"SYSMODE,CMDMODE,FRAME,YAW,PITCH,ROLL,GYROX,GYROY,GYROZ,ACCX,ACCY,ACCZ,SEN_TIME_MS,SEN_TIME_US,SEN_S2S_TIME_MS,SEN_S2S_TIME_US\n");
		spnUtilsWriteToFile(OutputSensorFile,
				",,,DEGREES,DEGREES,DEGREES,DEG/S,DEG/S,DEG/S,G,G,G,MSEC,USEC,MSEC,USEC\n");

		isFirstOutput = false;
	}

	// write data to file
	char buf[1024];
	sprintf(buf, "%s,%s,%i,%.1f,%.1f,%.1f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%u,%u,%u,%u\n",
			spnModeGetString(),
			spnCommandGetModeString(),
			spnSchedulerGetFrameCount(),
			Yaw,
			Pitch,
			Roll,
			GyroInRaw[X_AXIS],
			GyroInRaw[Y_AXIS],
			GyroInRaw[Z_AXIS],
			AccelInRaw[X_AXIS],
			AccelInRaw[Y_AXIS],
			AccelInRaw[Z_AXIS],
			senElapsedMSec,
			senElapsedUSec,
			senS2SElapsedMSec,
			senS2SElapsedUSec);

	spnUtilsWriteToFile(OutputSensorFile, buf);
}

static void userOutputOnExit(void)
{
	printf("Closing Output Files...\n");
	spnUtilsCloseOutputFile(OutputFile);
	spnUtilsCloseOutputFile(OutputSensorFile);
}

