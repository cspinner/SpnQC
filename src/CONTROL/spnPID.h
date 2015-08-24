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

	bool configure(float minOut, float maxOut, float interval, float KpIn, float KiIn, float KdIn);
	void setGains(float KpIn, float KiIn, float KdIn);
	float update(float setPoint, float feedback);

private:
	float Kp;
	float Ki;
	float Kd;
	float MinOut;
	float MaxOut;
	float Interval;
};



#endif /* CONTROL_SPNPID_H_ */
