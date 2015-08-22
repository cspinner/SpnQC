/*
 * spnMotorManager.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnPID.h"

#define PR_RATE_MIN_DEGPS -5.0
#define PR_RATE_MAX_DEGPS  5.0

typedef enum
{
	CMD_MODE_STANDBY_E,
	CMD_MODE_OPEN_LOOP_E,
	CMD_MODE_CLOSED_LOOP_E,
	CMD_MODE_STOP_E,
	CMD_MODE_CALIBRATE_HIGH_E,
	CMD_MODE_CALIBRATE_LOW_E,
	CMD_MODE_CALIBRATE_CENTER_E,
	CMD_MODE_COUNT_E
} SpnCommand_Mode_Type;

const char* CMD_MODE_STRINGS[CMD_MODE_COUNT_E] =
{
		//CMD_MODE_STANDBY_E:
		"CMD STANDBY MODE",

		//CMD_MODE_OPEN_LOOP_E:
		"CMD OPEN LOOP",

		//CMD_MODE_CLOSED_LOOP_E:
		"CMD CLOSED LOOP",

		//CMD_MODE_STOP_E:
		"CMD STOP MODE",

		//CMD_MODE_CALIBRATE_HIGH_E:
		"CMD CAL HIGH MODE",

		//CMD_MODE_CALIBRATE_LOW_E:
		"CMD CAL LOW MODE",

		//CMD_MODE_CALIBRATE_CENTER_E:
		"CMD CAL CENTER MODE"
};

static SpnCommand_Mode_Type commandMode = CMD_MODE_STANDBY_E;
static SpnPID pitchRatePID;
static SpnPID pitchAnglePID;
static SpnPID rollRatePID;
static SpnPID rollAnglePID;

bool spnCommandInit(void)
{
	float pid_interval = MINOR_FRAME_TIME_USEC/1000000.0;

	// Configure controller PIDs
	if( (pitchRatePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS) &&
	    (pitchAnglePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS) &&
		(rollRatePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS) &&
		(rollAnglePID.configure(PR_RATE_MIN_DEGPS, PR_RATE_MAX_DEGPS, pid_interval) == SUCCESS))
	{
		spnMotorsInit();

		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

void spnCommandUpdate(void)
{
	static System_Mode_Type previousMode = MODE_INIT_E;

	System_Mode_Type currentMode = spnModeGet();
	char userInput = spnUserInputCharGet(false);

	// Set command mode based on system mode
	switch(currentMode)
	{
		case MODE_RUN_E:
			if(previousMode != currentMode) commandMode = CMD_MODE_CLOSED_LOOP_E;

			switch(userInput)
			{
				case 'o':
				case 'O':
					commandMode = CMD_MODE_OPEN_LOOP_E;
					break;

				case 'p':
				case 'P':
					commandMode = CMD_MODE_CLOSED_LOOP_E;
					break;

				default:
					break;
			}
			break;

		case MODE_CALIBRATE_E:
			if(previousMode != currentMode) commandMode = CMD_MODE_CALIBRATE_HIGH_E;

			switch(userInput)
			{
				case 'l':
				case 'L':
					commandMode = CMD_MODE_CALIBRATE_LOW_E;
					break;

				case 'h':
				case 'H':
					commandMode = CMD_MODE_CALIBRATE_HIGH_E;
					break;

				case 'c':
				case 'C':
					commandMode = CMD_MODE_CALIBRATE_CENTER_E;
					break;

				default:
					break;
			}
			break;

		case MODE_STANDBY_E:
			if(previousMode != currentMode) commandMode = CMD_MODE_STANDBY_E;
			break;

		case MODE_STOP_E:
		default:
			commandMode = CMD_MODE_STOP_E;
			break;
	}
	static int width=1060;
	static bool up = true;

	// Process command mode
	switch(commandMode)
	{
		case CMD_MODE_OPEN_LOOP_E:
			break;

		case CMD_MODE_CLOSED_LOOP_E:
//	float pitchRateCommand = PitchAnglePID.update(0.0, SpnPitchAngleSense);
//	float motorCommand = PitchRatePID.update(pitchRateCommand, SpnPitchRateSense);
			break;

		case CMD_MODE_CALIBRATE_HIGH_E:
//			printf("Command motors high\n");
//			spnMotorsSet(0, 100.0);
//			spnServoblasterSet(2,1860);
			spnServoblasterSet(2,width);

			if(width >= 1460)
			{
				up = false;
			}
			else if(width <= 1160)
			{
				up = true;
			}

			if(up) width+=10;
			else width-=10;


//			spnMotorsSet(1, 100.0);
//			spnMotorsSet(2, 100.0);
//			spnMotorsSet(3, 100.0);
			break;

		case CMD_MODE_CALIBRATE_LOW_E:
//			printf("Command motors low\n");
//			spnMotorsSet(0, 1.0);
			spnServoblasterSet(2,1060);
//			spnMotorsSet(1, 1.0);
//			spnMotorsSet(2, 1.0);
//			spnMotorsSet(3, 1.0);
			break;

		case CMD_MODE_CALIBRATE_CENTER_E:
//			printf("Command motors low\n");
//			spnMotorsSet(0, 1.0);
			spnServoblasterSet(2,1460);
//			spnMotorsSet(1, 1.0);
//			spnMotorsSet(2, 1.0);
//			spnMotorsSet(3, 1.0);
			break;

		case CMD_MODE_STANDBY_E:
		case CMD_MODE_STOP_E:
		default:
			// tbd - issue motor stop command
			break;
	}

	// Save current mode
	previousMode = currentMode;
}

const char* spnCommandGetModeString(void)
{
	return CMD_MODE_STRINGS[commandMode];
}



