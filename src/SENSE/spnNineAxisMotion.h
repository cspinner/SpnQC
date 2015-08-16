/*
 * spnNineAxisMotion.h
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#ifndef SENSE_SPNNINEAXISMOTION_H_
#define SENSE_SPNNINEAXISMOTION_H_

#include "spnSensor.h"

typedef struct
{
	// These biases will be added to input data
	struct
	{
		float x_bias; // g
		float y_bias; // g
		float z_bias; // g
	} accel;

	// These biases will be added to input data
	struct
	{
		float x_bias; // º/s
		float y_bias; // º/s
		float z_bias; // º/s
	} gyro;

	struct
	{
		// compensates for hard errors:
		float x_bias; // mG
		float y_bias; // mG
		float z_bias; // mG

		// compensates for soft errors:
		float x_scale;
		float y_scale;
		float z_scale;
	} mag;
} SpnNineAxisMotion_Calibration_Type;

typedef struct
{
	// SPI
	int chipSelect;
	int speed;

	// Filtering
	int rollingAvgCount;
	float magOutlierThresh;

	// Calibration
	SpnNineAxisMotion_Calibration_Type* pCal;
} SpnNineAxisMotion_Cfg_Type;

typedef struct
{
	struct
	{
		float x; // g
		float y; // g
		float z; // g
	} accel;

	struct
	{
		float x; // º/s
		float y; // º/s
		float z; // º/s
	} gyro;

	struct
	{
		float x; // mG
		float y; // mG
		float z; // mG
	} mag;

	float temperature; // ºF
} SpnNineAxisMotion_Data_Type;

// For the MPU-9250 sensor
class SpnNineAxisMotion : public SpnSensor
{

public:
	SpnNineAxisMotion(void);

	bool configure(void* cfg);

	void acquireData(void);

	bool retrieveData(int* size, void* data);

private:
	char readRegister(int address);
	void writeRegister(int address, char data);
	char readRegisterMask(int address, char mask);
	void writeRegisterMask(int address, char mask, char data);
	char readMagRegister(char address);
	void writeMagRegister(char address, char data);
	void writeMagRegisterMask(int address, char mask, char data);
	void readMagRegisterSet(int startAddress, char readCount, char* pOut);
	void applyCalibration(void);
};



#endif /* SENSE_SPNNINEAXISMOTION_H_ */
