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

// assumes one floating point value per line
static bool readNextFloatFromFile(FILE* pFile, float* pDest)
{
	size_t bytesRead;
	char readBytes[128];

	// Read the line
	bytesRead = spnUtilsReadLine(pFile, readBytes, sizeof(readBytes));

	if(bytesRead > 0)
	{
		// Parse the float value
		sscanf(readBytes, "%f", pDest);

		return SUCCESS;
	}
	else
	{
		// some error or EOF
		return FAIL;
	}
}

static bool initMPU9250(void)
{
	SpnNineAxisMotion_Cfg_Type cfg_mpu9250;
	SpnNineAxisMotion_Calibration_Type cal_mpu9250;
	FILE* pInputFile = NULL; // input file descriptor

	// Set configuration data
	cfg_mpu9250.chipSelect = 0;
	cfg_mpu9250.speed = 500000;
	cfg_mpu9250.rollingAvgCount = 5;
	cfg_mpu9250.magOutlierThresh = 50.0;
	cfg_mpu9250.pCal = &cal_mpu9250;

	//
	// ATTEMPT TO READ CALIBRATION FILE
	//

	// open the file
	spnUtilsOpenFileForRead(&pInputFile, "calibration");

	// If file exists, parse the data and populate cal_mpu9250
	if(pInputFile != NULL)
	{
		// "calibration" has the format (all floats, one per line):
		//   accel x bias
		//   accel y bias
		//   accel z bias
		//   gyro x bias
		//   gyro y bias
		//   gyro z bias
		//   mag x bias
		//   mag y bias
		//   mag z bias
		//   mag x scale
		//   mag y scale
		//   mag z scale
		bool status = SUCCESS;

		// accel
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.accel.x_bias) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.accel.y_bias) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.accel.z_bias) == SUCCESS) ? status : FAIL;

		// gyro
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.gyro.x_bias) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.gyro.y_bias) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.gyro.z_bias) == SUCCESS) ? status : FAIL;

		// mag
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.mag.x_bias) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.mag.y_bias) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.mag.z_bias) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.mag.x_scale) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.mag.y_scale) == SUCCESS) ? status : FAIL;
		status = (readNextFloatFromFile(pInputFile, &cal_mpu9250.mag.z_scale) == SUCCESS) ? status : FAIL;

		// close the file
		spnUtilsCloseFile(pInputFile);

		// Enter calibration if data was not completely read
		if(status != SUCCESS)
		{
			spnModeSet(MODE_CALIBRATE_E);
		}
	}
	else
	{
		// Otherwise, enter calibration mode
		spnModeSet(MODE_CALIBRATE_E);

		// Cal data was not read. Default to no calibration
		cal_mpu9250.accel.x_bias = 0;
		cal_mpu9250.accel.y_bias = 0;
		cal_mpu9250.accel.z_bias = 0;
		cal_mpu9250.gyro.x_bias  = 0;
		cal_mpu9250.gyro.y_bias  = 0;
		cal_mpu9250.gyro.z_bias  = 0;
		cal_mpu9250.mag.x_bias = 0;
		cal_mpu9250.mag.y_bias = 0;
		cal_mpu9250.mag.z_bias = 0;
		cal_mpu9250.mag.x_scale = 1;
		cal_mpu9250.mag.y_scale = 1;
		cal_mpu9250.mag.z_scale = 1;
	}

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
	printf("Initialize MPU9250...");
	status = initMPU9250();

	// Madgwick AHRS coefficient
	beta = 1000;

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
	Yaw   += 7.62; // Declination at Charlotte, NC is 7° 37' W on 2015-08-05
	Roll  = rollRad * 180.0 / PI;

	// In this installation, roll is situated at exactly 180 degrees at rest which causes the
	// reading to jump around between ~180 and ~-180. Compensate for this:
	if(Roll < 0) Roll += 180.0;
	else if(Roll > 0) Roll -= 180.0;
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
