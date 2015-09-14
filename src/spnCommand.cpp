/*
 * spnMotorManager.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnPID.h"
#include <stdlib.h>

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
static SpnPID yawRatePID;
static SpnPID yawAnglePID;
static bool motorEnabled[4];

static void setCommandMode(void);
static void processCommandMode(void);

bool spnCommandInit(void)
{
	float32_t pid_interval = MINOR_FRAME_TIME_USEC/1000000.0;

	const SpnQC_Config_Type* const pCfg = spnConfigGet();
    
    if(spnServoInit(pCfg->transceiver.chanCount, &pCfg->transceiver.gpioPin[0], pCfg->motor.chanCount, &pCfg->motor.gpioPin[0]) == EXIT_SUCCESS)
    {
        // Configure controller PIDs, Motor Outputs, Transceiver Inputs
        if( (pitchRatePID.configure(pCfg->command.pidOutMin, pCfg->command.pidOutMax, pid_interval,
                pCfg->command.pidPitchKp, pCfg->command.pidPitchKi, pCfg->command.pidPitchKd) == EXIT_SUCCESS) &&
            (pitchAnglePID.configure(pCfg->command.pidOutMin, pCfg->command.pidOutMax, pid_interval,
                    pCfg->command.pidPitchKp, pCfg->command.pidPitchKi, pCfg->command.pidPitchKd) == EXIT_SUCCESS) &&
            (rollRatePID.configure(pCfg->command.pidOutMin, pCfg->command.pidOutMax, pid_interval,
                    pCfg->command.pidRollKp, pCfg->command.pidRollKi, pCfg->command.pidRollKd) == EXIT_SUCCESS) &&
            (rollAnglePID.configure(pCfg->command.pidOutMin, pCfg->command.pidOutMax, pid_interval,
                    pCfg->command.pidRollKp, pCfg->command.pidRollKi, pCfg->command.pidRollKd) == EXIT_SUCCESS) &&
            (yawRatePID.configure(pCfg->command.pidOutMin, pCfg->command.pidOutMax, pid_interval,
                    pCfg->command.pidYawKp, pCfg->command.pidYawKi, pCfg->command.pidYawKd) == EXIT_SUCCESS) &&
            (yawAnglePID.configure(pCfg->command.pidOutMin, pCfg->command.pidOutMax, pid_interval,
                    pCfg->command.pidYawKp, pCfg->command.pidYawKi, pCfg->command.pidYawKd) == EXIT_SUCCESS) &&
            (spnMotorsInit() == EXIT_SUCCESS) &&
            (spnTransceiverInit() == EXIT_SUCCESS))
        {
        	for(uint32_t i = 0; i < pCfg->motor.chanCount; i++)
        	{
        		motorEnabled[i] = pCfg->motor.motorEnable[i];
        	}

            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
	else
	{
		return EXIT_FAILURE;
	}
}

void spnCommandUpdate(void)
{
	setCommandMode();
	processCommandMode();
}

const char* spnCommandGetModeString(void)
{
	return CMD_MODE_STRINGS[commandMode];
}

static void setCommandMode(void)
{
	static System_Mode_Type previousMode = MODE_INIT_E;

	char userInput = spnUserInputCharGet(false);
	System_Mode_Type currentMode = spnModeGet();

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

	// Save current mode
	previousMode = currentMode;
}

static void processCommandMode(void)
{
	float32_t throttlePct = spnTransceiverGetThrottlePct();
	float32_t elevatorAngle = spnTransceiverGetElevatorAngle();
	float32_t aileronAngle = spnTransceiverGetAileronAngle();
	float32_t rudderAngle = spnTransceiverGetRudderAngle();

	float32_t rollPidOut = 0.0;
	float32_t pitchPidOut = 0.0;
	float32_t yawPidOut = 0.0;

	float32_t yaw;
	float32_t pitch;
	float32_t roll;

	// retrieve IMU data
	spnSensorGetPrincipalAxes(&pitch, &roll, &yaw);

	// Process command mode
	switch(commandMode)
	{
		case CMD_MODE_OPEN_LOOP_E:
			// No PID
			break;

		case CMD_MODE_CLOSED_LOOP_E:
			if(throttlePct > 0.0)
			{
				pitchPidOut = pitchAnglePID.update(elevatorAngle, pitch);
				rollPidOut = rollAnglePID.update(aileronAngle, roll);
				yawPidOut = yawAnglePID.update(rudderAngle, yaw);
			}
			break;

		case CMD_MODE_CALIBRATE_HIGH_E:
			spnMotorsCalibrateDrive(0);
			break;

		case CMD_MODE_CALIBRATE_LOW_E:
			spnMotorsCalibrateDrive(2);
			break;

		case CMD_MODE_CALIBRATE_CENTER_E:
			spnMotorsCalibrateDrive(1);
			break;

		case CMD_MODE_STANDBY_E:
		case CMD_MODE_STOP_E:
		default:
			spnMotorsStopAll();
			break;
	}

	if((CMD_MODE_OPEN_LOOP_E == commandMode) ||
       (CMD_MODE_CLOSED_LOOP_E == commandMode))
	{
		// Set output
		yawPidOut = 0; // tbd
		if(motorEnabled[0]) spnMotorsSet(0, clamp(throttlePct + pitchPidOut + yawPidOut, 0.0, 100.0));
		if(motorEnabled[1]) spnMotorsSet(1, clamp(throttlePct + rollPidOut - yawPidOut, 0.0, 100.0));
		if(motorEnabled[2]) spnMotorsSet(2, clamp(throttlePct - pitchPidOut + yawPidOut, 0.0, 100.0));
		if(motorEnabled[3]) spnMotorsSet(3, clamp(throttlePct - rollPidOut - yawPidOut, 0.0, 100.0));
	}
}


