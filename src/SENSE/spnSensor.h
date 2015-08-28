/*
 * spnSensor.h
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#ifndef SENSE_SPNSENSOR_H_
#define SENSE_SPNSENSOR_H_

class SpnSensor
{
public:
	SpnSensor(void);

	bool configure(void);
	bool configure(void* cfg);

	void acquireData(void);
	void acquireData(void* opt);

	bool retrieveData(uint32_t* size, void* data);
	bool retrieveData(void* opt, uint32_t* size, void* data);

	bool getStatus(void);

protected:
	void setStatus(bool status);

private:
	bool isConfigured;
	bool sensorStatus;
};



#endif /* SENSE_SPNSENSOR_H_ */
