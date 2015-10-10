/*
 * utils.cpp
 *
 *  Created on: Jul 14, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "OSAL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool spnUtilsTimedOut(OSAL_Time_Type* pTsEnd)
{
	OSAL_Time_Type tsNow;

	OSAL_TIME_OF_DAY(&tsNow);

	// Check for timeout
	if(spnUtilsTimeCompare(&tsNow, pTsEnd) == true)
	{
		return true; // Timed Out
	}
	else
	{
		return false;
	}
}

// delayUsec range 0 - 1000000
void spnUtilsWaitUsec(uint32_t delayUsec)
{
	OSAL_Time_Type tsNow;
	OSAL_Time_Type tsAdder;
	OSAL_Time_Type tsEnd;

	// Compute end time
	OSAL_TIME_OF_DAY(&tsNow);
	tsAdder.seconds = 0;
	tsAdder.microSeconds = delayUsec;
	OSAL_TIME_ADD(&tsNow, &tsAdder, &tsEnd);

	while(!spnUtilsTimedOut(&tsEnd)) {};
}

OSAL_Time_Type spnUtilsAddToTimestamp(OSAL_Time_Type* pTimeStamp, uint32_t sec, uint32_t usec)
{
	OSAL_Time_Type tsAdder;
	OSAL_Time_Type tsEnd = *pTimeStamp;

	// Compute end time
	tsAdder.seconds = sec;
	tsAdder.microSeconds = usec;
	OSAL_TIME_ADD(&tsEnd, &tsAdder, &tsEnd);

	return tsEnd;
}

void spnUtilsMarkTimestamp(OSAL_Time_Type* pTimeStamp)
{
	OSAL_TIME_OF_DAY(pTimeStamp);
}

// Returns time since pTimeStamp
OSAL_Time_Type spnUtilsGetElapsedTime(OSAL_Time_Type* pTimeStamp)
{
	OSAL_Time_Type tsNow = {0};
	OSAL_Time_Type tsElapsed = {0};

	// Get elapsed time since timestamp
	OSAL_TIME_OF_DAY(&tsNow);
	OSAL_TIME_SUB(&tsNow, pTimeStamp, &tsElapsed);

	return tsElapsed;
}

// Compare A > B
bool spnUtilsTimeCompare(OSAL_Time_Type* pTsA, OSAL_Time_Type* pTsB)
{
	return (((pTsA)->seconds == (pTsB)->seconds) ?
			((pTsA)->microSeconds > (pTsB)->microSeconds) : ((pTsA)->seconds > (pTsB)->seconds));
}

bool spnUtilsOpenFileForRead(uint32_t* pFileId, const char *pPathname)
{
	return OSAL_INPUT_FILE_OPEN(pFileId, strcat(WORKING_DIRECTORY,pPathname));
}

uint32_t spnUtilsReadLine(uint32_t fileId, char* pDest, size_t destSizeBytes)
{
	uint32_t bytesRead = 0;

	OSAL_INPUT_FILE_READ_LINE(fileId, pDest, destSizeBytes, &bytesRead);

	return bytesRead;
}

// assumes one floating point value per line
bool spnUtilsReadNextFloatFromFile(uint32_t fileId, float32_t* pDest)
{
	size_t bytesRead;
	char readBytes[128];

	// Read the line
	bytesRead = spnUtilsReadLine(fileId, readBytes, sizeof(readBytes));

	if(bytesRead > 0)
	{
		// Parse the float32_t value
		sscanf(readBytes, "%f", pDest);

		return EXIT_SUCCESS;
	}
	else
	{
		// some error or EOF
		return EXIT_FAILURE;
	}
}

// assumes one integer point value per line
bool spnUtilsReadNextIntFromFile(uint32_t fileId, int32_t* pDest)
{
	size_t bytesRead;
	char readBytes[128];

	// Read the line
	bytesRead = spnUtilsReadLine(fileId, readBytes, sizeof(readBytes));

	if(bytesRead > 0)
	{
		// Parse the float32_t value
		sscanf(readBytes, "%i", pDest);

		return EXIT_SUCCESS;
	}
	else
	{
		// some error or EOF
		return EXIT_FAILURE;
	}
}

bool spnUtilsOpenFileForAppend(uint32_t* pFileId, const char *pPathname)
{
	return OSAL_OUTPUT_FILE_OPEN(pFileId, strcat(WORKING_DIRECTORY,pPathname), OSAL_FILE_APPEND_E);
}

bool spnUtilsCreateFileForWrite(uint32_t* pFileId, const char *pPathname)
{
	return OSAL_OUTPUT_FILE_OPEN(pFileId, strcat(WORKING_DIRECTORY,pPathname), OSAL_FILE_CREATE_E);
}

void spnUtilsWriteToFile(uint32_t fileId, const char *pBuf)
{
	OSAL_OUTPUT_FILE_WRITE(fileId, pBuf);
}

void spnUtilsCloseOutputFile(uint32_t fileId)
{
	OSAL_OUTPUT_FILE_CLOSE(fileId);
}

void spnUtilsCloseInputFile(uint32_t fileId)
{
	OSAL_INPUT_FILE_CLOSE(fileId);
}
