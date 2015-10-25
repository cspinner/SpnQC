/*
 * spnTransceiver.cpp
 *
 *  Created on: Aug 22, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "HAL.h"
#include <stdlib.h>

static bool overrideModeEnabled = false;

bool spnTransceiverInit(void)
{
    const SpnQC_Config_Type* const pCfg = spnConfigGet();
    
    overrideModeEnabled = pCfg->transceiver.useTerminal ||
    				      pCfg->transceiver.useNetworkInput;

    // Initialize servo inputs
    HAL_SERVO_INPUT_INIT(pCfg->transceiver.chanCount, &pCfg->transceiver.gpioPin[0]);

	return EXIT_SUCCESS;
}

float32_t spnTransceiverGetThrottlePct(void)
{
    float32_t throttlePct = 0.0;
    
    if(overrideModeEnabled)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == '[') ovrInput -= 2.0;
        else if (userInput == ']') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, 0.0, 90.0);
        
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

    if(overrideModeEnabled)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 't') ovrInput -= 1.0;
		else if (userInput == 'y') ovrInput += 1.0;
		else if (userInput == 'u') ovrInput -= 45.0;
		else if (userInput == 'i') ovrInput += 45.0;

        ovrInput = clamp(ovrInput, -45.0, 45.0);
        
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

    if(overrideModeEnabled)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 'g') ovrInput -= 1.0;
        else if (userInput == 'b') ovrInput += 1.0;
        else if (userInput == 'f') ovrInput -= 45.0;
        else if (userInput == 'v') ovrInput += 45.0;

        ovrInput = clamp(ovrInput, -45.0, 45.0);
        
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

    if(overrideModeEnabled)
    {
        static float32_t ovrInput = 0.0;

        char userInput = spnUserInputCharGet(false);

        if(userInput == 'u') ovrInput -= 2.0;
        else if (userInput == 'i') ovrInput += 2.0;

        ovrInput = clamp(ovrInput, -90.0, 90.0);
        
        rudAngle = ovrInput;
    }
    else
    {
        // tbd - use receiver
    }
    
	return rudAngle;
}
