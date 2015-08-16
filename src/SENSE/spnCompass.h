/*
 * spnCompass.h
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#include "spnSensor.h"

#ifndef SENSE_SPNCOMPASS_H_
#define SENSE_SPNCOMPASS_H_

// For the CMPS03 sensor
class SpnCompass : public SpnSensor
{
public:
	SpnCompass(void);

	bool configure(void);

	void acquireData(void);

	bool retrieveData(int* size, void* data);

private:
	int i2c_fd;
	char version; // compass version
	short dataRaw; // 0-3599 for a full circle, representing 0-359.9 degrees.
	float dataConverted; // 0-359.9 degrees
};

#endif /* SENSE_SPNCOMPASS_H_ */
