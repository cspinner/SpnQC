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

	bool retrieveData(uint32_t* size, void* data);

private:
	int32_t i2c_fd;
	uint8_t version; // compass version
	int16_t dataRaw; // 0-3599 for a full circle, representing 0-359.9 degrees.
	float32_t dataConverted; // 0-359.9 degrees
};

#endif /* SENSE_SPNCOMPASS_H_ */
