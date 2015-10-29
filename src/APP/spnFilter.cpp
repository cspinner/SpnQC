/*
 * spnFilter.cpp
 *
 *  Created on: Sep 13, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnFilter.h"
#include <stdint.h>
#include <stdlib.h>

// FILT_LP_2P_7HZBR_200HZSMP
static float32_t lp2_7_200(float32_t rawData[], uint32_t dataCount, float32_t filtStateX[], float32_t filtStateY[]); // 200hz

// FILT_ZERO_REJECT
static float32_t zeroReject(float32_t rawData[], uint32_t dataCount, float32_t filtStateX[], float32_t filtStateY[]);

SpnFilter::SpnFilter(void)
{
	pFilterFunc = NULL;
}

bool SpnFilter::configure(SpnFilter_Select_Type filterSel)
{
	switch (filterSel)
	{
		case FILT_LP_2P_7HZBR_200HZSMP:
			pFilterFunc = &lp2_7_200;
			break;

		case FILT_ZERO_REJECT:
			pFilterFunc = &zeroReject;
			filtStateX[0] = 0;
			break;

		default:
			break;
	}

	return EXIT_SUCCESS;
}

float32_t SpnFilter::update(float32_t rawData[], uint32_t dataCount)
{
	return pFilterFunc(rawData, dataCount, filtStateX, filtStateY);
}

// Low pass filter - 2 poles, cutoff @ 7Hz - sample rate 200hz
static float32_t lp2_7_200(float32_t rawData[], uint32_t dataCount, float32_t gxv[], float32_t gyv[]) // 200hz
{
	#define GAIN   9.585509742e+01

	float32_t filtData = 0.0;

	if(dataCount > 0)
	{
		for(uint32_t i = 0; i < dataCount; i++)
		{
			gxv[0] = gxv[1]; gxv[1] = gxv[2];
			gxv[2] = rawData[i] / GAIN;
			gyv[0] = gyv[1]; gyv[1] = gyv[2];
			gyv[2] =   (gxv[0] + gxv[2]) + 2 * gxv[1]
						 + ( -0.7327260304 * gyv[0]) + (  1.6909963769 * gyv[1]);
			filtData = gyv[2];
		}
	}
	else
	{
		filtData = gyv[2]; // maintain last value
	}

	return filtData;
}

// Zero reject filter
static float32_t zeroReject(float32_t rawData[], uint32_t dataCount, float32_t gxv[], float32_t gyv[]) // 200hz
{
	float32_t filtData = 0.0;

	// If current and previous data is zero
	if((rawData[0] == 0) && (gxv[0] == 0))
	{
		filtData = 0.0;
	}
	// If current is zero, but previous is non-zero
	else if((rawData[0] == 0) && (gxv[0] != 0))
	{
		filtData = gxv[0];
	}
	// If current is non-zero
	else
	{
		filtData = rawData[0];
	}

	// Update previous data
	gxv[0] = rawData[0];

	return filtData;
}
//
//// Low pass filter - 2 poles, cutoff @ 4Hz - use for Gyro
//static void lp2_4(SpnNineAxisMotion_Raw_Data_Type rawData[], SpnNineAxisMotion_Raw_Data_Type* filtData, uint32_t dataCount)
//{
//	#define NZEROS 2
//	#define NPOLES 2
//	#define GAIN   2.761148367e+02
//
//	static float gxv[NZEROS+1][3], gyv[NPOLES+1][3];
//
//	for(uint32_t i = 0; i < dataCount; i++)
//	{
//		for(int32_t axis = 0; axis < 3; axis++)
//		{
//			float32_t* pRawData;
//			float32_t* pFiltData;
//
//			if(axis == 0) { pRawData = &rawData[i].x_raw; pFiltData = &filtData->x_raw; }
//			if(axis == 1) { pRawData = &rawData[i].y_raw; pFiltData = &filtData->y_raw; }
//			if(axis == 2) { pRawData = &rawData[i].z_raw; pFiltData = &filtData->z_raw; }
//
//			gxv[0][axis] = gxv[1][axis]; gxv[1][axis] = gxv[2][axis];
//			gxv[2][axis] = *pRawData / GAIN;
//			gyv[0][axis] = gyv[1][axis]; gyv[1][axis] = gyv[2][axis];
//			gyv[2][axis] =   (gxv[0][axis] + gxv[2][axis]) + 2 * gxv[1][axis]
//						 + ( -0.8371816513 * gyv[0][axis]) + (  1.8226949252 * gyv[1][axis]);
//			*pFiltData = gyv[2][axis];
//		}
//	}
//}
//
//// Low pass filter - 4 poles, cutoff @ 3Hz - use for Gyro
//static void lp4_3(SpnNineAxisMotion_Raw_Data_Type rawData[], SpnNineAxisMotion_Raw_Data_Type* filtData, uint32_t dataCount)
//{
//	#define NZEROS 4
//	#define NPOLES 4
//	#define GAIN   2.286922409e+05
//
//	static float gxv[NZEROS+1][3], gyv[NPOLES+1][3];
//
//	for(uint32_t i = 0; i < dataCount; i++)
//	{
//		for(uint32_t axis = 0; axis < 3; axis++)
//		{
//			float32_t* pRawData;
//			float32_t* pFiltData;
//
//			if(axis == 0) { pRawData = &rawData[i].x_raw; pFiltData = &filtData->x_raw; }
//			if(axis == 1) { pRawData = &rawData[i].y_raw; pFiltData = &filtData->y_raw; }
//			if(axis == 2) { pRawData = &rawData[i].z_raw; pFiltData = &filtData->z_raw; }
//
//			gxv[0][axis] = gxv[1][axis]; gxv[1][axis] = gxv[2][axis]; gxv[2][axis] = gxv[3][axis]; gxv[3][axis] = gxv[4][axis];
//			gxv[4][axis] = *pRawData / GAIN;
//			gyv[0][axis] = gyv[1][axis]; gyv[1][axis] = gyv[2][axis]; gyv[2][axis] = gyv[3][axis]; gyv[3][axis] = gyv[4][axis];
//			gyv[4][axis] =   (gxv[0][axis] + gxv[4][axis]) + 4 * (gxv[1][axis] + gxv[3][axis]) + 6 * gxv[2][axis]
//						 + ( -0.7816187403 * gyv[0][axis]) + (  3.3189386048 * gyv[1][axis])
//						 + ( -5.2911525842 * gyv[2][axis]) + (  3.7537627567 * gyv[3][axis]);
//			*pFiltData = gyv[4][axis];
//		}
//	}
//}
//
//// Low pass filter - 4 poles, cutoff @ 1Hz - use for Accel
//static void lp4_1(SpnNineAxisMotion_Raw_Data_Type rawData[], SpnNineAxisMotion_Raw_Data_Type* filtData, uint32_t dataCount)
//{
//	#define NZEROS 4
//	#define NPOLES 4
//	#define GAIN   1.710822297e+07
//
//	static float gxv[NZEROS+1][3], gyv[NPOLES+1][3];
//
//	for(uint32_t i = 0; i < dataCount; i++)
//	{
//		for(int32_t axis = 0; axis < 3; axis++)
//		{
//			float32_t* pRawData;
//			float32_t* pFiltData;
//
//			if(axis == 0) { pRawData = &rawData[i].x_raw; pFiltData = &filtData->x_raw; }
//			if(axis == 1) { pRawData = &rawData[i].y_raw; pFiltData = &filtData->y_raw; }
//			if(axis == 2) { pRawData = &rawData[i].z_raw; pFiltData = &filtData->z_raw; }
//
//			gxv[0][axis] = gxv[1][axis]; gxv[1][axis] = gxv[2][axis]; gxv[2][axis] = gxv[3][axis]; gxv[3][axis] = gxv[4][axis];
//			gxv[4][axis] = *pRawData / GAIN;
//			gyv[0][axis] = gyv[1][axis]; gyv[1][axis] = gyv[2][axis]; gyv[2][axis] = gyv[3][axis]; gyv[3][axis] = gyv[4][axis];
//			gyv[4][axis] =   (gxv[0][axis] + gxv[4][axis]) + 4 * (gxv[1][axis] + gxv[3][axis]) + 6 * gxv[2][axis]
//						 + ( -0.9211819292 * gyv[0][axis]) + (  3.7603495077 * gyv[1][axis])
//						 + ( -5.7570763791 * gyv[2][axis]) + (  3.9179078654 * gyv[3][axis]);
//			*pFiltData = gyv[4][axis];
//		}
//	}
//}
