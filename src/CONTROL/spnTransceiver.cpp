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

float spnTransceiverGetThrottlePct(void)
{
    float throttlePct = 0.0;
    
    if(terminalOverride)
    {
        static float ovrInput = 0.0;

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
float spnTransceiverGetElevatorAngle(void)
{
	float elevAngle = 0.0;

    if(terminalOverride)
    {
        static float ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 'q') ovrInput -= 2.0;
        else if (userInput == 'w') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, 0.0, 100.0);
        
        elevAngle = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }

	return elevAngle;
}

// Determines Change In Roll
float spnTransceiverGetAileronAngle(void)
{
	static float ailAngle = 0.0;

    if(terminalOverride)
    {
        static float ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 'e') ovrInput -= 2.0;
        else if (userInput == 'r') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, 0.0, 100.0);
        
        ailAngle = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }

	return ailAngle;
}

// Determines Change In Yaw
float spnTransceiverGetRudderAngle(void)
{
	static float rudAngle = 0.0;

    if(terminalOverride)
    {
        static float ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 't') ovrInput -= 2.0;
        else if (userInput == 'y') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, 0.0, 100.0);
        
        rudAngle = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }
    
	return rudAngle;
}
