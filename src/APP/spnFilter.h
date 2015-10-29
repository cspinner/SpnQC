/*
 * spnFilter.h
 *
 *  Created on: Sep 13, 2015
 *      Author: cspinner
 */

#ifndef SPNFILTER_H_
#define SPNFILTER_H_

#include "SpnQC.h"
#include <stdint.h>
#include <stdlib.h>

#define MAX_POLES_ZEROS 4

typedef enum
{
	FILT_LP_2P_7HZBR_200HZSMP,
	FILT_ZERO_REJECT,
	FILT_COUNT
} SpnFilter_Select_Type;

typedef float32_t (*FilterFuncType)(float32_t[], uint32_t, float32_t[], float32_t[]);

class SpnFilter
{
public:
	SpnFilter(void);

	bool configure(SpnFilter_Select_Type filterSel);
	float32_t update(float32_t rawData[], uint32_t dataCount);

private:
	FilterFuncType pFilterFunc;
	float32_t filtStateX[MAX_POLES_ZEROS+1]; // filter state
	float32_t filtStateY[MAX_POLES_ZEROS+1]; // filter state
};



#endif /* SPNFILTER_H_ */
