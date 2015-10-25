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
    void getGains(float32_t* KpOut, float32_t* KiOut, float32_t* KdOut);
    void getTerms(float32_t* pOut, float32_t* iOut, float32_t* dOut, float32_t* sumOut);
	float32_t update(float32_t setPoint, float32_t feedback);

private:
	float32_t m_Kp;
	float32_t m_Ki;
	float32_t m_Kd;
	float32_t m_MinOut;
	float32_t m_MaxOut;
	float32_t m_Proportional;
	float32_t m_Interval;
	float32_t m_Derivative;
	float32_t m_Output;
    
    float32_t m_ErrorPrev;
    float32_t m_Integral;
};



#endif /* CONTROL_SPNPID_H_ */
