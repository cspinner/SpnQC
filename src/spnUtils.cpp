/*
 * utils.cpp
 *
 *  Created on: Jul 14, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max input file buffer
#define BUF_SIZE 8192

bool spnUtilsTimedOut(struct timeval* pTsEnd)
{
	struct timeval tsNow;

	gettimeofday(&tsNow, 0);

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
	struct timeval tsNow;
	struct timeval tsAdder;
	struct timeval tsEnd;

	// Compute end time
	gettimeofday(&tsNow, 0);
	tsAdder.tv_sec = 0;
	tsAdder.tv_usec = delayUsec;
	timeradd(&tsNow, &tsAdder, &tsEnd);

	while(!spnUtilsTimedOut(&tsEnd)) {};
}

struct timeval spnUtilsAddToTimestamp(struct timeval* pTimeStamp, __time_t sec, __suseconds_t usec)
{
	struct timeval tsAdder;
	struct timeval tsEnd = *pTimeStamp;

	// Compute end time
	tsAdder.tv_sec = sec;
	tsAdder.tv_usec = usec;
	timeradd(&tsEnd, &tsAdder, &tsEnd);

	return tsEnd;
}

void spnUtilsMarkTimestamp(struct timeval* pTimeStamp)
{
	gettimeofday(pTimeStamp, 0);
}

// Returns time since pTimeStamp
struct timeval spnUtilsGetElapsedTime(struct timeval* pTimeStamp)
{
	struct timeval tsNow = {0};
	struct timeval tsElapsed = {0};

	// Get elapsed time since timestamp
	gettimeofday(&tsNow, 0);
	timersub(&tsNow, pTimeStamp, &tsElapsed);

	return tsElapsed;
}

// Compare A > B
bool spnUtilsTimeCompare(struct timeval* pTsA, struct timeval* pTsB)
{
	return (((pTsA)->tv_sec == (pTsB)->tv_sec) ?
			((pTsA)->tv_usec > (pTsB)->tv_usec) : ((pTsA)->tv_sec > (pTsB)->tv_sec));
}
//#include <unistd.h>
void spnUtilsOpenFileForRead(FILE **pFile, const char *pPathname)
{
//	char cwd[1024];
//	   if (getcwd(cwd, sizeof(cwd)) != NULL)
//	       printf("Current working dir: %s\n", cwd);
//
//	   system("ls -l /home/pi/calibration");

	// open the file for reading
	*pFile = fopen(pPathname, "r");
//
//
//		perror("OpenFileForRead Error");
}


size_t spnUtilsReadLine(FILE *pFile, char* pDest, size_t destSizeBytes)
{
	char* readBuffer = pDest;
	char* rtnBuf;
	size_t readBufferSize = destSizeBytes;
	_IO_ssize_t bytesRead;

	rtnBuf = fgets(readBuffer, readBufferSize, pFile);

	bytesRead = strlen(readBuffer);

	return bytesRead;
}

// assumes one float32_ting point value per line
bool spnUtilsReadNextFloatFromFile(FILE* pFile, float32_t* pDest)
{
	size_t bytesRead;
	char readBytes[128];

	// Read the line
	bytesRead = spnUtilsReadLine(pFile, readBytes, sizeof(readBytes));

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
bool spnUtilsReadNextIntFromFile(FILE* pFile, int32_t* pDest)
{
	size_t bytesRead;
	char readBytes[128];

	// Read the line
	bytesRead = spnUtilsReadLine(pFile, readBytes, sizeof(readBytes));

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

void spnUtilsOpenFileForAppend(FILE **pFile, const char *pPathname)
{
	*pFile = fopen(pPathname, "a");

	if(*pFile == NULL)
	{
		perror("OpenFileForAppend Error");
	}
}

void spnUtilsCreateFileForWrite(FILE **pFile, const char *pPathname)
{
	*pFile = fopen(pPathname, "w");

	if(*pFile == NULL)
	{
		perror("CreateFileForWrite Error");
	}
}

void spnUtilsWriteToFile(FILE *pFile, const char *pBuf)
{
	size_t bytesWritten = fputs(pBuf, pFile);

	if(bytesWritten == EOF)
	{
		perror("WriteToFile Error");
	}
}

void spnUtilsCloseFile(FILE *pFile)
{
	if(fclose(pFile) != 0)
	{
		perror("CloseFile Error");
	}
}
