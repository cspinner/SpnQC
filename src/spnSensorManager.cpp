/*
 * spnSensorManager.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnNineAxisMotion.h"
#include <string.h>
#include <math.h>

extern "C"
{
	#include "MadgwickAHRS.h"
};

using namespace std;

static SpnNineAxisMotion MPU9250;
static SpnNineAxisMotion_Data_Type SpnNineAxisMotionData;
static float Pitch, Yaw, Roll;

static bool initMPU9250(void)
{
	const SpnQC_Config_Type* const pCfg = spnConfigGet();

	SpnNineAxisMotion_Cfg_Type cfg_mpu9250;

	// Copy config data
	cfg_mpu9250.chipSelect = pCfg->spi.chipSelect;
	cfg_mpu9250.speed = pCfg->spi.speed;
	cfg_mpu9250.accFsSel = pCfg->mpu9250.accFsSel;
	cfg_mpu9250.gyroFsSel = pCfg->mpu9250.gyroFsSel;
	cfg_mpu9250.magOutlierThresh = pCfg->mpu9250.magOutlierThresh;
	cfg_mpu9250.rollingAvgCount = pCfg->mpu9250.rollingAvgCount;
	cfg_mpu9250.calibration.accel.x_bias = pCfg->mpu9250.accel.x_bias;
	cfg_mpu9250.calibration.accel.y_bias = pCfg->mpu9250.accel.y_bias;
	cfg_mpu9250.calibration.accel.z_bias = pCfg->mpu9250.accel.z_bias;
	cfg_mpu9250.calibration.gyro.x_bias = pCfg->mpu9250.gyro.x_bias;
	cfg_mpu9250.calibration.gyro.y_bias = pCfg->mpu9250.gyro.y_bias;
	cfg_mpu9250.calibration.gyro.z_bias = pCfg->mpu9250.gyro.z_bias;
	cfg_mpu9250.calibration.mag.x_bias = pCfg->mpu9250.mag.x_bias;
	cfg_mpu9250.calibration.mag.y_bias = pCfg->mpu9250.mag.y_bias;
	cfg_mpu9250.calibration.mag.z_bias = pCfg->mpu9250.mag.z_bias;
	cfg_mpu9250.calibration.mag.x_scale = pCfg->mpu9250.mag.x_scale;
	cfg_mpu9250.calibration.mag.y_scale = pCfg->mpu9250.mag.y_scale;
	cfg_mpu9250.calibration.mag.z_scale = pCfg->mpu9250.mag.z_scale;

	beta = pCfg->mpu9250.beta;

	return MPU9250.configure(&cfg_mpu9250);
}

static void quaternionToEuler(float* quat0, float* quat1, float* quat2, float* quat3,
						      float* yawRad, float* pitchRad, float* rollRad)
{
	float sqx = (*quat1)*(*quat1);
	float sqy = (*quat2)*(*quat2);
	float sqz = (*quat3)*(*quat3);
	float asinfInput;

	asinfInput = -2.f * ((*quat0)*(*quat2) - (*quat1)*(*quat3));
	if(asinfInput > 1) asinfInput = 1; // protects against NaN - clamp to 90 deg max
	if(asinfInput < -1) asinfInput = -1; // protects against NaN - clamp to -90 deg min
	*pitchRad = asinf(asinfInput);
    *rollRad = atan2f(2.f *   ((*quat0)*(*quat1) + (*quat2)*(*quat3)), 1.f - 2.f*(sqx + sqy));
	*yawRad = atan2f(2.f *  ((*quat0)*(*quat3) + (*quat1)*(*quat2)), 1.f - 2.f*(sqy + sqz));
}

bool spnSensorManagerInit(void)
{
	bool status = FAIL;

	// Configure sensors
	status = initMPU9250();

	return status;
}

void spnSensorManagerUpdate(void)
{
	int phNineAxisDataSize;
	float yawRad, pitchRad, rollRad;

	// Acquire Sensor Data
	MPU9250.acquireData();

	// Convert and Retrieve Sensor Data
	spnStatusSet(MPU9250.retrieveData(&phNineAxisDataSize, &SpnNineAxisMotionData));

	//
	// MADGWICK
	//
	// Update quaternions
	MadgwickAHRSupdateIMU(
				SpnNineAxisMotionData.gyro.x,
				SpnNineAxisMotionData.gyro.y,
				SpnNineAxisMotionData.gyro.z,
				SpnNineAxisMotionData.accel.x,
				SpnNineAxisMotionData.accel.y,
				SpnNineAxisMotionData.accel.z);

	// Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
	// In this coordinate system, the positive z-axis is down toward Earth.
	// Yaw is the angle between Sensor x-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
	// Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
	// Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
	// These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
	// Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
	// applied in the correct order which for this configuration is yaw, pitch, and then roll.
	// For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
	quaternionToEuler(&q0, &q1, &q2, &q3, &yawRad, &pitchRad, &rollRad);

	// Radians to Degrees
	Pitch = pitchRad * 180.0 / PI;
	Yaw   = yawRad * 180.0 / PI;
	Roll  = rollRad * 180.0 / PI;

	// In this installation, roll is situated at exactly 180 degrees at rest which causes the
	// reading to jump around between ~180 and ~-180. Compensate for this:
	if(Roll < 0) Roll += 180.0;
	else if(Roll > 0) Roll -= 180.0;
//
//	// In this installation, yaw progresses from 0 to 180, then -180 to 0. Compensate for this:
//	Yaw = (Yaw + 180.0)/2.0;
}

void spnSensorGetPrincipalAxes(float* pPitch, float* pRoll, float* pYaw)
{
	*pPitch = Pitch;
	*pRoll = Roll;
	*pYaw = Yaw;
}

float spnSensorGetTemperature(void)
{
	return SpnNineAxisMotionData.temperature;
}