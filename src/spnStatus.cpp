/*
 * spnStatus.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"

static bool spnGlobalStatus = FAIL;

bool spnStatusGet(void)
{
	return spnGlobalStatus;
}

void spnStatusSet(bool state)
{
	spnGlobalStatus = state;
}


