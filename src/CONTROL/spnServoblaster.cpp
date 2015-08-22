/*
 * spnServoblaster.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"
#include <stdio.h>

void spnServoblasterSet(int index, int pulseWidthUsec)
{
	FILE* sbWriteFile;
	char buf[1024];
	sprintf(buf, "%i=%ius\n", index, pulseWidthUsec);

	spnUtilsCreateFileForWrite(&sbWriteFile, "/dev/servoblaster");

	if(!sbWriteFile)
	{
		printf("Error creating file: /dev/servoblaster.\n");
	}

	spnUtilsWriteToFile(sbWriteFile, buf);
	spnUtilsCloseFile(sbWriteFile);
}


