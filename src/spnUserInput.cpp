/*
 * spnUserInput.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnConfig.h"
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static bool useTerminal = false;
static bool useNetwork = false;
static bool isTerminalStateSet = false;
static struct termios ttystateold;
static char charInput = 0;
static timeval tsHeartbeatInterval;
static timeval tsHeartbeat;

static uint32_t getKeyboardHit(void);
static void setTerminalState(void);
static void restoreTerminalState(void);

bool spnUserInputInit(void)
{
	const SpnQC_Config_Type* const pCfg = spnConfigGet();

	useTerminal = pCfg->transceiver.useTerminal;
	useNetwork = pCfg->transceiver.useNetwork;

	tsHeartbeatInterval.tv_sec = pCfg->transceiver.netHeartbeatInterval.tv_sec;
	tsHeartbeatInterval.tv_usec = 0;
	spnUtilsMarkTimestamp(&tsHeartbeat); // initial heartbeat timestamp

	// Inputs come from RPI2 terminal
	if(useTerminal)
	{
		// Set terminal to non-canonical mode, no echo
		setTerminalState();

		if(isTerminalStateSet)
		{
			atexit(&restoreTerminalState);

			return EXIT_SUCCESS;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	// Inputs come from a networked ground station
	else if(useNetwork)
	{
		if(spnServerInit(pCfg->transceiver.netPort) == EXIT_SUCCESS)
		{
			spnServerWaitForGroundStation();
			return EXIT_SUCCESS;
		}
		else
		{
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
		if(getKeyboardHit() != 0)
		{
			charInput = fgetc(stdin);
		}
	}
	else if(useNetwork)
	{
		spnServerReadMessage(&charInput, 1);
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
	struct timeval tsElapsed = spnUtilsGetElapsedTime(&tsHeartbeat);

	// Check for input from client
	if(charInput != 0)
	{
		// record new timestamp
		spnUtilsMarkTimestamp(&tsHeartbeat);
		return EXIT_SUCCESS;
	}
	else // no input received
	{
		// no input received in configured seconds?
		if((tsElapsed.tv_sec >= tsHeartbeatInterval.tv_sec) && (tsElapsed.tv_usec >= 0))
		{
			// timed out
			return EXIT_FAILURE;
		}
		else
		{
			// still ok...
			return EXIT_SUCCESS;
		}
	}
}

static uint32_t getKeyboardHit(void)
{
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);

	return FD_ISSET(STDIN_FILENO, &fds);
}

static void setTerminalState(void)
{
	if(!isTerminalStateSet)
	{
		struct termios ttystate;

		//get the terminal state
		tcgetattr(STDIN_FILENO, &ttystate);

		// Save to "old" state data struct to be used for restore
		ttystateold = ttystate;

		//turn off canonical mode and echo
		ttystate.c_lflag &= ~(ICANON|ECHO|ECHOE|ECHOK|ECHONL);

		//minimum of number input read.
		ttystate.c_cc[VMIN] = 1;

		//set the terminal attributes.
		tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

		isTerminalStateSet = true;
	}
}

static void restoreTerminalState(void)
{
	if(isTerminalStateSet)
	{
		//restore the terminal attributes.
		printf("Restoring STDIN state...\n");
		tcsetattr(STDIN_FILENO, TCSANOW, &ttystateold);
	}
}
