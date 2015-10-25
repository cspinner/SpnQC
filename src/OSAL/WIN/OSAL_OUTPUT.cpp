/*
 * OSAL_OUTPUT.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: cspinner
 */

#include "OSAL.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

static uint32_t FileArrayNextIndex = 0;
static FILE* FilePtrArray[32];

bool OSAL_OUTPUT_FILE_OPEN(uint32_t* pFileId, const char *pPathname, OSAL_Output_File_Type type)
{
	// Create and open? Or open for append?
	if(type == OSAL_FILE_CREATE_E)
	{
		FilePtrArray[FileArrayNextIndex] = fopen(pPathname, "w");
	}
	else // OSAL_FILE_APPEND_E
	{
		FilePtrArray[FileArrayNextIndex] = fopen(pPathname, "a");
	}

	if(FilePtrArray[FileArrayNextIndex] == NULL)
	{
		perror("OpenFileForWrite Error");

		return EXIT_FAILURE;
	}
	else
	{
		*pFileId = FileArrayNextIndex;
		FileArrayNextIndex++;

		return EXIT_SUCCESS;
	}
}

bool OSAL_OUTPUT_FILE_WRITE(uint32_t fileId, const char *pBuf)
{
	size_t bytesWritten = fputs(pBuf, FilePtrArray[fileId]);

	if(bytesWritten == EOF)
	{
		perror("WriteToFile Error");

		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}

bool OSAL_OUTPUT_FILE_CLOSE(uint32_t fileId)
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



