/*
 * spnTransceiver.cpp
 *
 *  Created on: Aug 22, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"

static bool terminalOverride = false;

bool spnTransceiverInit(void)
{
    const SpnQC_Config_Type* const pCfg = spnConfigGet();
    
    terminalOverride = pCfg->transceiver.useTerminal;
    
	return SUCCESS;
}

float32_t spnTransceiverGetThrottlePct(void)
{
    float32_t throttlePct = 0.0;
    
    if(terminalOverride)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == '[') ovrInput -= 2.0;
        else if (userInput == ']') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, 0.0, 100.0);
        
        throttlePct = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }

	return throttlePct;
}

// Determines Change In Pitch
float32_t spnTransceiverGetElevatorAngle(void)
{
	float32_t elevAngle = 0.0;

    if(terminalOverride)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 'q') ovrInput -= 2.0;
        else if (userInput == 'w') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, -90.0, 90.0);
        
        elevAngle = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }

	return elevAngle;
}

// Determines Change In Roll
float32_t spnTransceiverGetAileronAngle(void)
{
	static float32_t ailAngle = 0.0;

    if(terminalOverride)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 'e') ovrInput -= 2.0;
        else if (userInput == 'r') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, -90.0, 90.0);
        
        ailAngle = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }

	return ailAngle;
}

// Determines Change In Yaw
float32_t spnTransceiverGetRudderAngle(void)
{
	static float32_t rudAngle = 0.0;

    if(terminalOverride)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 't') ovrInput -= 2.0;
        else if (userInput == 'y') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, -90.0, 90.0);
        
        rudAngle = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }
    
	return rudAngle;
}
