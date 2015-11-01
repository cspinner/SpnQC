#include "HAL_private.h"
#include <stdlib.h>
#include <stdio.h>

extern "C"
{
	#include "pigpiod_if.h"
}

#define GPIO_COUNT (PI_MAX_USER_GPIO+1)
#define END_OF_LIST -1
#define LEVEL_FALLING 0
#define LEVEL_RISING 1

typedef struct
{    
    uint32_t tickUsec[2]; // [0] = falling edge, [1] = rising edge
    uint32_t tickDiffUsec;
} Servo_Pulse_Record_Type;

static uint32_t gpioInputs[GPIO_COUNT];
static uint32_t gpioInputCount;
static uint32_t inputCallbackIds[GPIO_COUNT];
Servo_Pulse_Record_Type pulseRecords[GPIO_COUNT];

static uint32_t gpioOutputs[GPIO_COUNT];
static uint32_t gpioOutputCount;
static uint32_t gpioOutputPulses[GPIO_COUNT];

static bool isDaemonStarted = false;

static void servoInOnExit(void);
static void servoOutOnExit(void);
static void servoInputCbEx(uint32_t gpioNum, uint32_t level, uint32_t tick, void* gpioIndex);

bool HAL_SERVO_INPUT_INIT(uint32_t inputCount, const uint32_t* gpioInputList)
{
	int32_t rtnVal;

    // Connect to the daemon (localhost) if not already started
	if(!isDaemonStarted)
	{
	    rtnVal = pigpio_start(NULL, NULL);
		if(rtnVal < 0)
		{
			printf("spnServoInit: pigpio_start - %s\n", pigpio_error(rtnVal));
			return EXIT_FAILURE;
		}

		isDaemonStarted = true;
	}

	// register for exit callback to disconnect from the daemon
	atexit(&servoInOnExit);

	if(gpioInputList != NULL)
	{
		// set each gpio to an input with pull-down resistor
		for (uint32_t index = 0; index < inputCount; index++)
		{
			rtnVal = set_mode(gpioInputList[index], PI_INPUT);

			if(rtnVal != 0)
			{
				printf("spnServoInit: set_mode - %s\n", pigpio_error(rtnVal));
				return EXIT_FAILURE;
			}
			else
			{
				// Set pull-down
				set_pull_up_down(gpioInputList[index], PI_PUD_DOWN);

				// register callback
				inputCallbackIds[index] = callback_ex(gpioInputList[index], EITHER_EDGE, &servoInputCbEx, (void*)index);
			}

			gpioInputs[index] = gpioInputList[index];
			gpioInputCount++;
		}
	}

	return EXIT_SUCCESS;
}

bool HAL_SERVO_OUTPUT_INIT(uint32_t outputCount, const uint32_t* gpioOutputList)
{
	int32_t rtnVal;

    // Connect to the daemon (localhost) if not already started
	if(!isDaemonStarted)
	{
		rtnVal = pigpio_start(NULL, NULL);
		if(rtnVal < 0)
		{
			printf("spnServoInit: pigpio_start - %s\n", pigpio_error(rtnVal));
			return EXIT_FAILURE;
		}

		isDaemonStarted = true;
	}

	// register for exit callback to disconnect from the daemon
	atexit(&servoOutOnExit);

	if(gpioOutputList != NULL)
	{
		// set each gpio to an output
		for (uint32_t index = 0; index < outputCount; index++)
		{
			rtnVal = set_mode(gpioOutputList[index], PI_OUTPUT);

			if(rtnVal != 0)
			{
				printf("spnServoInit: set_mode - %s\n", pigpio_error(rtnVal));
				return EXIT_FAILURE;
			}

			gpioOutputs[index] = gpioOutputList[index];
			gpioOutputCount++;
		}
	}

	return EXIT_SUCCESS;
}

static void servoInOnExit(void)
{
	printf("Disconnecting Servos Input ...\n");

	// cancel any registered callbacks
	for(uint32_t i = 0; i < gpioInputCount; i++)
	{
		callback_cancel(inputCallbackIds[i]);
	}

	if(isDaemonStarted)
	{
		// disconnect from the daemon if still running
    	pigpio_stop();
    	isDaemonStarted = false;
    }
}

static void servoOutOnExit(void)
{
	printf("Disconnecting Servos Output ...\n");
    
    // disconnect from the daemon if still running
    if(isDaemonStarted)
    {
    	 // stop all outputs
		HAL_SERVO_PULSES_STOP();

    	pigpio_stop();
    	isDaemonStarted = false;
    }
}

/*
 * INPUTS
 */
bool HAL_SERVO_PULSE_SAMPLE(uint32_t gpioIndex)
{
    // Ensure that we've captured a complete pulse
    if(pulseRecords[gpioIndex].tickUsec[LEVEL_FALLING] > pulseRecords[gpioIndex].tickUsec[LEVEL_RISING])
    {
        // 32-bit counter roll-over is OK since we're doing unsigned subtraction...     
        pulseRecords[gpioIndex].tickDiffUsec = pulseRecords[gpioIndex].tickUsec[LEVEL_FALLING] -
                                               pulseRecords[gpioIndex].tickUsec[LEVEL_RISING];

        // Reset for next time
        pulseRecords[gpioIndex].tickUsec[LEVEL_FALLING] = 0;
        pulseRecords[gpioIndex].tickUsec[LEVEL_RISING] = 0;
    }
    else
    {
        pulseRecords[gpioIndex].tickDiffUsec = PI_SERVO_OFF;
    }

    return EXIT_SUCCESS;
}

bool HAL_SERVO_PULSE_WIDTH_GET(uint32_t gpioIndex, uint32_t* pPulseWidth)
{
    *pPulseWidth = pulseRecords[gpioIndex].tickDiffUsec;

    return EXIT_SUCCESS;
}

// handle level change and compute time between rise and fall
static void servoInputCbEx(uint32_t gpioNum, uint32_t level, uint32_t tick, void* gpioIndex)
{
    // As tick is an unsigned 32 bit quantity it wraps around after 2**32 microseconds, 
    //   which is approximately 1 hour 12 minutes.
	pulseRecords[(uint32_t)gpioIndex].tickUsec[level] = tick;
}

/*
 * OUTPUTS
 */
bool HAL_SERVO_PULSE_WIDTH_SET(uint32_t gpioIndex, uint32_t pulseWidthUsec)
{
    // write pulse width to the output gpio
    int32_t rtnVal = set_servo_pulsewidth(gpioOutputs[gpioIndex], pulseWidthUsec);
                   
    if(rtnVal != 0)
    {
        printf("spnServoSetPulseWidth: set_servo_pulsewidth - %s\n", pigpio_error(rtnVal));
        return EXIT_FAILURE;
    }
    else
    {
    	gpioOutputPulses[gpioIndex] = pulseWidthUsec;
        return EXIT_SUCCESS;
    }
}

bool HAL_SERVO_GET_COMMANDED_PULSE_WIDTH(uint32_t gpioIndex, uint32_t* pPulseWidth)
{
	*pPulseWidth = gpioOutputPulses[gpioIndex];
	return EXIT_SUCCESS;
}

bool HAL_SERVO_PULSES_STOP(void)
{
    bool rtnStatus = EXIT_SUCCESS;
    
    if(isDaemonStarted)
    {
		// write pulse width of 0 to stop each of the output gpios
		for(uint32_t index = 0; index < gpioOutputCount; index++)
		{
			if(HAL_SERVO_PULSE_WIDTH_SET(index, PI_SERVO_OFF) != EXIT_SUCCESS)
			{
				rtnStatus = EXIT_FAILURE;
			}
		}
    }
    
    return rtnStatus;
}
