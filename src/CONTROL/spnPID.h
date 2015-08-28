/*
 * spnPID.h
 *
 *  Created on: Apr 1, 2015
 *      Author: cspinner
 */

#ifndef CONTROL_SPNPID_H_
#define CONTROL_SPNPID_H_

class SpnPID
{
public:
	SpnPID(void);

	bool configure(float32_t minOut, float32_t maxOut, float32_t interval, float32_t KpIn, float32_t KiIn, float32_t KdIn);
	void setGains(float32_t KpIn, float32_t KiIn, float32_t KdIn);
	float32_t update(float32_t setPoint, float32_t feedback);

private:
	float32_t Kp;
	float32_t Ki;
	float32_t Kd;
	float32_t MinOut;
	float32_t MaxOut;
	float32_t Interval;
};



#endif /* CONTROL_SPNPID_H_ */
