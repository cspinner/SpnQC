/*
 * spnSensorManager.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "HAL.h"
#include "spnFilter.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

extern "C"
{
	#include "MadgwickAHRS.h"
};

using namespace std;

static float32_t Pitch, Yaw, Roll;
static SpnFilter yawFilter;

static void quaternionToEuler(float32_t* quat0, float32_t* quat1, float32_t* quat2, float32_t* quat3,
						      float32_t* yawRad, float32_t* pitchRad, float32_t* rollRad)
{
	float32_t sqx = (*quat1)*(*quat1);
	float32_t sqy = (*quat2)*(*quat2);
	float32_t sqz = (*quat3)*(*quat3);
	float32_t asinfInput;

	asinfInput = -2.f * ((*quat0)*(*quat2) - (*quat1)*(*quat3));
	if(asinfInput > 1) asinfInput = 1; // protects against NaN - clamp to 90 deg max
	if(asinfInput < -1) asinfInput = -1; // protects against NaN - clamp to -90 deg min
	*pitchRad = asinf(asinfInput);
    *rollRad = atan2f(2.f *   ((*quat0)*(*quat1) + (*quat2)*(*quat3)), 1.f - 2.f*(sqx + sqy));
	*yawRad = atan2f(2.f *  ((*quat0)*(*quat3) + (*quat1)*(*quat2)), 1.f - 2.f*(sqy + sqz));
}

static void complementaryFilter(float gx, float gy, float gz,
								float ax, float ay, float az,
								float *pitch, float *roll,
								float gyroWt, float accelWt,
								float dt)
{
    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *roll += gx * dt; // Angle around the X-axis
    *pitch -= gy * dt;     // Angle around the Y-axis

    // Compensate for drift with accelerometer data (ignoring if bad accel data)
    float forceMagnitudeApprox = fabsf(ax) + fabsf(ay) + fabsf(az);
    if (forceMagnitudeApprox > 0.5 && forceMagnitudeApprox < 2.0)
    {
    	// Work out the squares
		float x2 = (ax*ax);
		float y2 = (ay*ay);
		float z2 = (az*az);

		//X Axis
		float result=sqrt(y2+z2);
		result=ax/result;
		float accel_angle_y = atanf(result) * 180 / M_PI;

		//Y Axis
		result=sqrt(x2+z2);
		result=ay/result;
		float accel_angle_x = atanf(result) * 180 / M_PI;

	// Turning around the X axis results in a vector on the Y-axis
        *roll = *roll * gyroWt + accel_angle_x * accelWt;

	// Turning around the Y axis results in a vector on the X-axis
        *pitch = *pitch * gyroWt + accel_angle_y * accelWt;
    }
}

bool spnSensorManagerInit(void)
{
	PRINT("start spnSensorManagerInit");

	bool status = EXIT_FAILURE;

	const SpnQC_Config_Type* const pCfg = spnConfigGet();

	// Configure sensors
	status = HAL_IMU_INIT();

	// Configure Yaw Filter
	yawFilter.configure(FILT_LP_2P_7HZBR_200HZSMP);

	// Set beta used in Madgwick
	beta = pCfg->mpu9250.beta;

	return status;
}

void spnSensorManagerPollSensors(void)
{
	// Acquire Sensor Data
	HAL_IMU_UPDATE();
}

void spnSensorManagerProcessData(void)
{
	float32_t yawRad, pitchRad, rollRad;
	float32_t accelIn[NUM_AXIS], gyroIn[NUM_AXIS], magIn[NUM_AXIS];

	// Retrieve the sensor data
	spnSensorGetNineAxesData(accelIn, gyroIn, magIn);

	//
	// MADGWICK
	//
	// Update quaternions
	MadgwickAHRSupdate(gyroIn[X_AXIS], gyroIn[Y_AXIS], gyroIn[Z_AXIS],
						  accelIn[X_AXIS], accelIn[Y_AXIS], accelIn[Z_AXIS],
						  magIn[X_AXIS], magIn[Y_AXIS], magIn[Z_AXIS]);

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
//	Pitch = pitchRad * 180.0 / PI;
	Yaw = yawRad * 180.0 / PI;
	Yaw = yawFilter.update(&Yaw, 1);
//	Roll  = rollRad * 180.0 / PI;
//
//	// In this installation, roll is situated at exactly 180 degrees at rest which causes the
//	// reading to jump around between ~180 and ~-180. Compensate for this:
//	if(Roll < 0) Roll += 180.0;
//	else if(Roll > 0) Roll -= 180.0;
//
//	// In this installation, yaw progresses from 0 to 180, then -180 to 0. Compensate for this:
//	Yaw = (Yaw + 180.0)/2.0;
	complementaryFilter(gyroIn[X_AXIS], gyroIn[Y_AXIS], gyroIn[Z_AXIS],
					    accelIn[X_AXIS], accelIn[Y_AXIS], accelIn[Z_AXIS],
						&Pitch, &Roll, 0.99, 0.01, 0.030);

}

void spnSensorGetPrincipalAxes(float32_t* pPitch, float32_t* pRoll, float32_t* pYaw)
{
	*pPitch = Pitch;
	*pRoll = Roll;
	*pYaw = Yaw;
}

void spnSensorGetNineAxesData(float32_t* accel, float32_t* gyro, float32_t* mag)
{
	HAL_IMU_READ(accel, gyro, mag);
}

void spnSensorGetRawNineAxesData(float32_t* accel, float32_t* gyro, float32_t* mag, uint32_t frame)
{
	HAL_IMU_READ_RAW(accel, gyro, mag, frame);
}

void spnSensorGetTemperature(float32_t* temperature)
{
	HAL_IMU_TEMPERATURE_GET(temperature);
}
