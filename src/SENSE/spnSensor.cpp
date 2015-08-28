/*
 * spnSensor.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnSensor.h"

using namespace std;

SpnSensor::SpnSensor(void)
{
	isConfigured = false;
	setStatus(FAIL);
}

bool SpnSensor::configure(void)
{
	isConfigured = true;
	setStatus(SUCCESS);
	return getStatus();
}
bool SpnSensor::configure(void* cfg)
{
	isConfigured = true;
	setStatus(SUCCESS);
	return getStatus();
}

void SpnSensor::acquireData(void){}
void SpnSensor::acquireData(void* opt){}

bool SpnSensor::retrieveData(uint32_t* size, void* data)
{
	*size = 0;
	return getStatus();
}
bool SpnSensor::retrieveData(void* opt, uint32_t* size, void* data)
{
	*size = 0;
	return getStatus();
}

bool SpnSensor::getStatus(void)
{
	return sensorStatus;
}

void SpnSensor::setStatus(bool status)
{
	sensorStatus = status;
}



