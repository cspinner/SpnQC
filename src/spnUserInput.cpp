/*
 * spnUserInput.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static bool isTerminalStateSet = false;
static struct termios ttystateold;
static char charInput = 0;

static uint32_t getKeyboardHit(void);
static void setTerminalState(void);
static void restoreTerminalState(void);

bool spnUserInputInit(void)
{
	// Set terminal to non-canonical mode, no echo
	setTerminalState();

	if(isTerminalStateSet)
	{
		atexit(&restoreTerminalState);

		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

void spnUserInputUpdate(void)
{
	if(getKeyboardHit() != 0)
	{
		charInput = fgetc(stdin);
	}
}

char spnUserInputCharGet(bool consume)
{
	char rtnChar = charInput;

	// Consume the input by clearing the character
	if(consume) charInput = 0;

	return rtnChar;
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
