/*
 * OSAL_INPUT.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: cspinner
 */

#include "OSAL.h"
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>

static bool isTerminalStateSet = false;

static uint32_t FileArrayNextIndex = 0;
static FILE* FilePtrArray[32];

static uint32_t getKeyboardHit(void);

bool OSAL_INPUT_KB_INIT(void)
{
	if(!isTerminalStateSet)
	{
		isTerminalStateSet = true;
	}

	return EXIT_SUCCESS;
}

bool OSAL_INPUT_KB_GET_HIT(char* pResult)
{
	if(getKeyboardHit() != 0)
	{
		*pResult = getch();
	}

	return EXIT_SUCCESS;
}

bool OSAL_INPUT_KB_RESTORE(void)
{
	if(isTerminalStateSet)
	{
		//restore the terminal attributes.
		printf("Restoring STDIN state...\n");
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

	// open the file for reading
	FilePtrArray[FileArrayNextIndex] = fopen(pPathname, "r");
	//
	//
	//		perror("OpenFileForRead Error");

	if(FilePtrArray[FileArrayNextIndex] == NULL)
	{
		perror("OpenFileForRead Error");

		return EXIT_FAILURE;
	}
	else
	{
		*pFileId = FileArrayNextIndex;
		FileArrayNextIndex++;

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
	return kbhit();
}



