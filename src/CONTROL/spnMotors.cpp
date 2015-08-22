/*
 * spnMotors.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"
#include <stdlib.h>

#define DC_MINIMUM_COMMAND 100.0 //tbd
#define DC_MAXIMUM_COMMAND 1000.0 //tbd
#define PWM_SUBCYCLE_TIME_USEC 500000
#define MOTOR_COUNT 4

typedef struct
{
	int motorIndex; // 0 <= motorNum < MOTOR_COUNT
	int dmaChan; // 0-14
	int gpio; // BCM pin numbering
} SpnMotor_Type;

static SpnMotor_Type spnMotorDefs[MOTOR_COUNT] =
{
	{
		0,
		0,
		24 // BCM 24, wiring pi 5
	},
	{
		1,
		1,
		24 // BCM 24, wiring pi 5
	},
	{
		2,
		2,
		24 // BCM 24, wiring pi 5
	},
	{
		3,
		3,
		24 // BCM 24, wiring pi 5
	},
};


static void spnMotorsOnExit(void)
{

}

void spnMotorsInit(void)
{

}

void spnMotorsSet(int motorNum, float cmdPct)
{

}


