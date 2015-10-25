/*
 * OSAL_INPUT.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */

#include "OSAL.h"
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool isTerminalStateSet = false;
static struct termios ttystateold;

static uint32_t FileArrayNextIndex = 0;
static FILE* FilePtrArray[32];

static uint32_t getKeyboardHit(void);

bool OSAL_INPUT_KB_INIT(void)
{
	// Set terminal to non-canonical mode, no echo
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

	return EXIT_SUCCESS;
}

bool OSAL_INPUT_KB_GET_HIT(char* pResult)
{
	if(getKeyboardHit() != 0)
	{
		*pResult = fgetc(stdin);
	}

	return EXIT_SUCCESS;
}

bool OSAL_INPUT_KB_RESTORE(void)
{
	if(isTerminalStateSet)
	{
		//restore the terminal attributes.
		printf("Restoring STDIN state...\n");
		tcsetattr(STDIN_FILENO, TCSANOW, &ttystateold);
	}

	return EXIT_SUCCESS;
}

//#include <unistd.h>
bool OSAL_INPUT_FILE_OPEN(uint32_t* pFileId, const char *pPathname)
{
	//	char cwd[1024];
	//	   if (getcwd(cwd, sizeof(cwd)) != NULL)
	//	       printf("Current working dir: %s\n", cwd);

	//	   system("ls -l /home/pi/calibration");
printf("a");
	// open the file for reading
	FilePtrArray[FileArrayNextIndex] = fopen(pPathname, "r");
	//
	//
	//		perror("OpenFileForRead Error");
	printf("b");
	if(FilePtrArray[FileArrayNextIndex] == NULL)
	{
		perror("OpenFileForRead Error");
		printf("c");
		return EXIT_FAILURE;
	}
	else
	{
		*pFileId = FileArrayNextIndex;
		FileArrayNextIndex++;
		printf("d");
		return EXIT_SUCCESS;
	}
}

bool OSAL_INPUT_FILE_READ_LINE(uint32_t fileId, char* pDest, uint32_t destSizeBytes, uint32_t* pBytesRead)
{
	char* readBuffer = pDest;
	char* rtnBuf;
	size_t readBufferSize = destSizeBytes;

	rtnBuf = fgets(readBuffer, readBufferSize, FilePtrArray[fileId]);

	*pBytesRead = strlen(readBuffer);

	return EXIT_SUCCESS;
}

bool OSAL_INPUT_FILE_CLOSE(uint32_t fileId)
{
	if(fclose(FilePtrArray[fileId]) != 0)
	{
		perror("CloseFile Error");

		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
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
