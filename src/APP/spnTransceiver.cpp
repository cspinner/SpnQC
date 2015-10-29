/*
 * spnTransceiver.cpp
 *
 *  Created on: Aug 22, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "HAL.h"
#include "spnFilter.h"
#include <stdlib.h>

#define PW2CMD(U,S) (((float32_t)U-S.intercept)/S.slope)

// (y - b) / m = c  --- linear relationship between command and pulse width
// command = (commanded pulse width - intercept) / slope
typedef struct
{
	uint32_t pinIdx;
	float32_t range;
	float32_t slope;
	float32_t intercept;
	uint32_t widthUsec;
} spnRxInputType;

enum
{
	TXR_THROTTLE_PIN_E,
	TXR_ELEVATOR_PIN_E,
	TXR_AILERON_PIN_E,
	TXR_RUDDER_PIN_E,
	TXR_NUM_PIN_E
};

static bool overrideModeEnabled = false;
static spnRxInputType txrCfg[TXR_NUM_PIN_E];
static SpnFilter zrFilter[TXR_NUM_PIN_E];

bool spnTransceiverInit(void)
{
    const SpnQC_Config_Type* const pCfg = spnConfigGet();
    
    overrideModeEnabled = pCfg->transceiver.useTerminal ||
    				      pCfg->transceiver.useNetworkInput;

    if(!overrideModeEnabled)
    {
    	txrCfg[TXR_THROTTLE_PIN_E].range = pCfg->transceiver.maxThrottleRngPct;
    	txrCfg[TXR_ELEVATOR_PIN_E].range = pCfg->transceiver.maxElevatorRngDeg;
    	txrCfg[TXR_AILERON_PIN_E].range = pCfg->transceiver.maxAileronRngDeg;
    	txrCfg[TXR_RUDDER_PIN_E].range = pCfg->transceiver.maxRudderRngDeg;

    	txrCfg[TXR_THROTTLE_PIN_E].pinIdx = 0;
    	txrCfg[TXR_ELEVATOR_PIN_E].pinIdx = 1;
    	txrCfg[TXR_AILERON_PIN_E].pinIdx = 2;
    	txrCfg[TXR_RUDDER_PIN_E].pinIdx = 3;

		for(uint32_t i = 0; i < TXR_NUM_PIN_E; i++)
		{
			txrCfg[i].slope = (pCfg->transceiver.pulseWidthFull - pCfg->transceiver.pulseWidthZero)/txrCfg[i].range;
			txrCfg[i].intercept = pCfg->transceiver.pulseWidthZero;
			zrFilter[i].configure(FILT_ZERO_REJECT);
		}

		// Initialize servo inputs
		HAL_SERVO_INPUT_INIT(pCfg->transceiver.chanCount, &pCfg->transceiver.gpioPin[0]);
    }

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
    	HAL_SERVO_PULSE_WIDTH_GET(txrCfg[TXR_THROTTLE_PIN_E].pinIdx, &txrCfg[TXR_THROTTLE_PIN_E].widthUsec);

    	txrCfg[TXR_THROTTLE_PIN_E].widthUsec =
    			zrFilter[TXR_THROTTLE_PIN_E].update((float32_t*)&txrCfg[TXR_THROTTLE_PIN_E].widthUsec, 1);

    	throttlePct = PW2CMD(txrCfg[TXR_THROTTLE_PIN_E].widthUsec, txrCfg[TXR_THROTTLE_PIN_E]);
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
    	HAL_SERVO_PULSE_WIDTH_GET(txrCfg[TXR_ELEVATOR_PIN_E].pinIdx, &txrCfg[TXR_ELEVATOR_PIN_E].widthUsec);

    	txrCfg[TXR_ELEVATOR_PIN_E].widthUsec =
    			zrFilter[TXR_ELEVATOR_PIN_E].update((float32_t*)&txrCfg[TXR_ELEVATOR_PIN_E].widthUsec, 1);

    	elevAngle = PW2CMD(txrCfg[TXR_ELEVATOR_PIN_E].widthUsec, txrCfg[TXR_ELEVATOR_PIN_E]) - txrCfg[TXR_ELEVATOR_PIN_E].range;
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
    	HAL_SERVO_PULSE_WIDTH_GET(txrCfg[TXR_AILERON_PIN_E].pinIdx, &txrCfg[TXR_AILERON_PIN_E].widthUsec);

    	txrCfg[TXR_AILERON_PIN_E].widthUsec =
    			zrFilter[TXR_AILERON_PIN_E].update((float32_t*)&txrCfg[TXR_AILERON_PIN_E].widthUsec, 1);

    	ailAngle = PW2CMD(txrCfg[TXR_AILERON_PIN_E].widthUsec, txrCfg[TXR_AILERON_PIN_E]) - txrCfg[TXR_AILERON_PIN_E].range;
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
    	HAL_SERVO_PULSE_WIDTH_GET(txrCfg[TXR_RUDDER_PIN_E].pinIdx, &txrCfg[TXR_RUDDER_PIN_E].widthUsec);

    	txrCfg[TXR_RUDDER_PIN_E].widthUsec =
    			zrFilter[TXR_RUDDER_PIN_E].update((float32_t*)&txrCfg[TXR_RUDDER_PIN_E].widthUsec, 1);

    	rudAngle = PW2CMD(txrCfg[TXR_RUDDER_PIN_E].widthUsec, txrCfg[TXR_RUDDER_PIN_E]) - txrCfg[TXR_RUDDER_PIN_E].range;
    }
    
	return rudAngle;
}

bool spnTransceiverIsActive(void)
{
	for(uint32_t i = 0; i < TXR_NUM_PIN_E; i++)
	{
		if(txrCfg[i].widthUsec == 0.0)
		{
			return false;
		}
	}

	return true;
}
