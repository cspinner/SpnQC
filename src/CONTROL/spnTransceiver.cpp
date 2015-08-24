/*
 * spnTransceiver.cpp
 *
 *  Created on: Aug 22, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"

bool spnTransceiverInit(void)
{
	return SUCCESS;
}

float spnTransceiverGetThrottlePct(void)
{
	static float ovrInput = 0.0;

	char userInput = spnUserInputCharGet(false);

	if(userInput == '[') ovrInput -= 2.0;
	else if (userInput == ']') ovrInput += 2.0;

	ovrInput = clamp(ovrInput, 0.0, 100.0);

	return ovrInput;
}

// Determines Change In Pitch
float spnTransceiverGetElevatorAngle(void)
{
	static float ovrInput = 0.0;

//	char userInput = spnUserInputCharGet(false);
//
//	if(userInput == '[') ovrInput -= 2.0;
//	else if (userInput == ']') ovrInput += 2.0;
//
//	ovrInput = clamp(ovrInput, 0.0, 100.0);

	return ovrInput;
}

// Determines Change In Roll
float spnTransceiverGetAileronAngle(void)
{
	static float ovrInput = 0.0;

//	char userInput = spnUserInputCharGet(false);
//
//	if(userInput == '[') ovrInput -= 2.0;
//	else if (userInput == ']') ovrInput += 2.0;
//
//	ovrInput = clamp(ovrInput, 0.0, 100.0);

	return ovrInput;
}

// Determines Change In Yaw
float spnTransceiverGetRudderAngle(void)
{
	static float ovrInput = 0.0;

//	char userInput = spnUserInputCharGet(false);
//
//	if(userInput == '[') ovrInput -= 2.0;
//	else if (userInput == ']') ovrInput += 2.0;
//
//	ovrInput = clamp(ovrInput, 0.0, 100.0);

	return ovrInput;
}
