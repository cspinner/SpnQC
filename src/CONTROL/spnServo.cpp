#include "SpnQC.h"
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
} SpnServo_Pulse_Record_Type;

static uint32_t gpioInputs[GPIO_COUNT];
static uint32_t gpioInputCount;
static uint32_t inputCallbackIds[GPIO_COUNT];
SpnServo_Pulse_Record_Type pulseRecords[GPIO_COUNT];

static uint32_t gpioOutputs[GPIO_COUNT];
static uint32_t gpioOutputCount;
static uint32_t gpioOutputPulses[GPIO_COUNT];

static void servoOnExit(void);
static void servoInputCbEx(uint32_t gpioNum, uint32_t level, uint32_t tick, void* gpioIndex);

bool spnServoInit(uint32_t inputCount, const uint32_t* gpioInputList, uint32_t outputCount, const uint32_t* gpioOutputList)
{
    // Connect to the daemon (localhost)
	int32_t rtnVal = pigpio_start(NULL, NULL);
    if(rtnVal < 0)
    {
        printf("spnServoInit: pigpio_start - %s\n", pigpio_error(rtnVal));
        return FAIL;
    }
    else
    {
        // register for exit callback to disconnect from the daemon
        atexit(&servoOnExit);
        
        if(gpioInputList != NULL)
        {
            // set each gpio to an input with pull-down resistor
            for (uint32_t index = 0; index < inputCount; index++)
            {
                rtnVal = set_mode(gpioInputList[index], PI_INPUT);
                   
                if(rtnVal != 0)
                {
                    printf("spnServoInit: set_mode - %s\n", pigpio_error(rtnVal));
                    return FAIL;
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
        
        if(gpioOutputList != NULL)
        {
            // set each gpio to an output
            for (uint32_t index = 0; index < outputCount; index++)
            {
                rtnVal = set_mode(gpioOutputList[index], PI_OUTPUT);
                   
                if(rtnVal != 0)
                {
                    printf("spnServoInit: set_mode - %s\n", pigpio_error(rtnVal));
                    return FAIL;
                }
                
                gpioOutputs[index] = gpioOutputList[index];
                gpioOutputCount++;
            }
        }
        
        return SUCCESS;
    }    
}

static void servoOnExit(void)
{
    // cancel any registered callbacks
    for(uint32_t i = 0; i < gpioInputCount; i++)
    {
        callback_cancel(inputCallbackIds[i]);
    }
    
    // stop all outputs
    spnServoStopAllPulses();
    
    // disconnect from the daemon
    pigpio_stop();
}

/*
 * INPUTS
 */
uint32_t spnServoGetPulseWidth(uint32_t gpioIndex)
{
    // Ensure that we've captured a complete pulse
    if(pulseRecords[gpioIndex].tickUsec[LEVEL_FALLING] > pulseRecords[gpioIndex].tickUsec[LEVEL_RISING])
    {
        // 32-bit counter roll-over is OK since we're doing unsigned subtraction...     
        pulseRecords[gpioIndex].tickDiffUsec = pulseRecords[gpioIndex].tickUsec[LEVEL_FALLING] -
                                               pulseRecords[gpioIndex].tickUsec[LEVEL_RISING];
    }
    else
    {
        pulseRecords[gpioIndex].tickDiffUsec = PI_SERVO_OFF;
    }
    
    return pulseRecords[gpioIndex].tickDiffUsec;
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
bool spnServoSetPulseWidth(uint32_t gpioIndex, uint32_t pulseWidthUsec)
{
    // write pulse width to the output gpio
    int32_t rtnVal = set_servo_pulsewidth(gpioOutputs[gpioIndex], pulseWidthUsec);
                   
    if(rtnVal != 0)
    {
        printf("spnServoSetPulseWidth: set_servo_pulsewidth - %s\n", pigpio_error(rtnVal));
        return FAIL;
    }
    else
    {
    	gpioOutputPulses[gpioIndex] = pulseWidthUsec;
        return SUCCESS;
    }
}

uint32_t spnServoGetCommandedPulseWidth(uint32_t gpioIndex)
{
	return gpioOutputPulses[gpioIndex];
}

bool spnServoStopAllPulses(void)
{
    bool rtnStatus = SUCCESS;
    
    // write pulse width of 0 to stop each of the output gpios
    for(uint32_t index = 0; index < gpioOutputCount; index++)
    {
        if(spnServoSetPulseWidth(index, PI_SERVO_OFF) != SUCCESS)
        {
            rtnStatus = FAIL;
        }
    }
    
    return rtnStatus;
}
