/*
 * spnSensor.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnSensor.h"
#include <stdlib.h>

using namespace std;

SpnSensor::SpnSensor(void)
{
	isConfigured = false;
	sensorStatus = EXIT_FAILURE;
}

bool SpnSensor::configure(void)
{
	isConfigured = true;
	sensorStatus = EXIT_SUCCESS;

	return EXIT_SUCCESS;
}
bool SpnSensor::configure(void* cfg)
{
	isConfigured = true;

	return EXIT_SUCCESS;
}

void SpnSensor::acquireData(void){}
void SpnSensor::acquireData(void* opt){}

bool SpnSensor::retrieveData(uint32_t* size, void* data)
{
	*size = 0;
	return EXIT_SUCCESS;
}
bool SpnSensor::retrieveData(void* opt, uint32_t* size, void* data)
{
	*size = 0;
	return EXIT_SUCCESS;
}

bool SpnSensor::getStatus(void)
{
	return sensorStatus;
}

void SpnSensor::setStatus(bool status)
{
	sensorStatus = status;
}



