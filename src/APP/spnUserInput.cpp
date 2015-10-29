/*
 * spnUserInput.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnConfig.h"
#include "OSAL.h"
#include <stdio.h>
#include <stdlib.h>

static bool useTerminal = false;
static bool useNetwork = false;
static bool useTransmitter = false;
static bool isCommEstablished = false;
static char charInput = 0;
static OSAL_Time_Type tsHeartbeatInterval;
static OSAL_Time_Type tsHeartbeat;

static void userInputRestore(void);

bool spnUserInputInit(void)
{
	PRINT("start spnUserInputInit");

	const SpnQC_Config_Type* const pCfg = spnConfigGet();
	useTerminal = pCfg->transceiver.useTerminal;
	useNetwork = pCfg->transceiver.useNetworkInput;
	useTransmitter = pCfg->transceiver.useTransmitter;

	tsHeartbeatInterval.seconds = pCfg->transceiver.netHeartbeatInterval.seconds;
	tsHeartbeatInterval.microSeconds = 0;

	// Inputs come from RPI2 terminal
	if(useTerminal)
	{
		if(spnTransceiverInit() == EXIT_SUCCESS)
		{
			// Set terminal to non-canonical mode, no echo
			OSAL_INPUT_KB_INIT();

			atexit(&userInputRestore);

			isCommEstablished = true;

			return EXIT_SUCCESS;
		}
		else
		{
			printf("spnTransceiverInit failed.\n");
			return EXIT_FAILURE;
		}
	}
	// Inputs come from a networked ground station
	else if(useNetwork)
	{
		if((spnTransceiverInit() == EXIT_SUCCESS) &&
		   (spnServerInit(pCfg->transceiver.netPort) == EXIT_SUCCESS))
		{
			return EXIT_SUCCESS;
		}
		else
		{
			printf("spnTransceiverInit or spnServerInit failed.\n");
			return EXIT_FAILURE;
		}
	}
	else if(useTransmitter)
	{
		if(spnTransceiverInit() == EXIT_SUCCESS)
		{
			return EXIT_SUCCESS;
		}
		else
		{
			printf("spnTransceiverInit failed.\n");
			return EXIT_FAILURE;
		}
	}
	else
	{
		printf("No input type configured.\n");
		return EXIT_FAILURE;
	}
}

void spnUserInputUpdate(void)
{
	if(useTerminal)
	{
		OSAL_INPUT_KB_GET_HIT(&charInput);
	}
	else if(useNetwork)
	{
		// Is it time to establish comm?
		if(spnModeGet() == MODE_ESTABLISH_COMM_E)
		{
			printf("Waiting for ground station connection...\n");
			spnHandleHaltTimer(); // temporarily halt timer interrupts before entering the blocking state
			spnServerWaitForGroundStation();
			spnUtilsMarkTimestamp(&tsHeartbeat); // initial heartbeat timestamp
			isCommEstablished = true;
			spnHandleStartTimer(); // restore timer interrupts
		}
		else
		{
			spnServerReadMessage(&charInput, 1);
			spnUserInputCheckHeartbeat();
		}
	}
	else if(useTransmitter)
	{
		// check for any activity from the transmitter/receiver
		if(spnTransceiverIsActive())
		{
			charInput = 0xFF;
		}

		spnUserInputCheckHeartbeat();
	}
}

char spnUserInputCharGet(bool consume)
{
	char rtnChar = charInput;

	// Consume the input by clearing the character
	if(consume) charInput = 0;

	return rtnChar;
}

bool spnUserInputCheckHeartbeat(void)
{
	OSAL_Time_Type tsElapsed = spnUtilsGetElapsedTime(&tsHeartbeat);

	// Check for input from client
	if(charInput != 0)
	{
		// record new timestamp
		spnUtilsMarkTimestamp(&tsHeartbeat);
		isCommEstablished = true;

		return EXIT_SUCCESS;
	}
	else // no input received
	{
		// no input received in configured seconds?
		if((tsElapsed.seconds >= tsHeartbeatInterval.seconds) && (tsElapsed.microSeconds >= 0))
		{
			// timed out
			isCommEstablished = false;

			return EXIT_FAILURE;
		}
		else
		{
			// still ok...
			return EXIT_SUCCESS;
		}
	}
}

bool spnUserInputCommEstablished(void)
{
	return isCommEstablished;
}

static void userInputRestore(void)
{
	OSAL_INPUT_KB_RESTORE();
}
