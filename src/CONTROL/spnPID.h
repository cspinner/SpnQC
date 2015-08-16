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

	bool configure(float minOut, float maxOut, float interval);
	float update(float setPoint, float feedback);

	float Kp;
	float Ki;
	float Kd;

private:
	float MinOut;
	float MaxOut;
	float Interval;
};



#endif /* CONTROL_SPNPID_H_ */
