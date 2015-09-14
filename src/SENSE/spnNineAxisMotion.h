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
		float32_t x_bias; // g
		float32_t y_bias; // g
		float32_t z_bias; // g
	} accel;

	// These biases will be added to input data
	struct
	{
		float32_t x_bias; // º/s
		float32_t y_bias; // º/s
		float32_t z_bias; // º/s
	} gyro;

	struct
	{
		// compensates for hard errors:
		float32_t x_bias; // mG
		float32_t y_bias; // mG
		float32_t z_bias; // mG

		// compensates for soft errors:
		float32_t x_scale;
		float32_t y_scale;
		float32_t z_scale;
	} mag;
} SpnNineAxisMotion_Calibration_Type;

typedef struct
{
	// SPI
	uint32_t chipSelect;
	uint32_t speed;

	// Filtering
	uint32_t accelFilterWindow;
	uint32_t gyroFilterWindow;
	uint32_t magFilterWindow;
	float32_t magOutlierThresh;

	// Sensor scale
	uint32_t accFsSel;
	uint32_t gyroFsSel;

	// Calibration
	SpnNineAxisMotion_Calibration_Type calibration;
} SpnNineAxisMotion_Cfg_Type;

typedef struct
{
	struct
	{
		float32_t x; // g
		float32_t y; // g
		float32_t z; // g
	} accel;

	struct
	{
		float32_t x; // º/s
		float32_t y; // º/s
		float32_t z; // º/s
	} gyro;

	struct
	{
		float32_t x; // mG
		float32_t y; // mG
		float32_t z; // mG
	} mag;

	float32_t temperature; // ºF
} SpnNineAxisMotion_Data_Type;

// For the MPU-9250 sensor
class SpnNineAxisMotion : public SpnSensor
{

public:
	SpnNineAxisMotion(void);

	bool configure(void* cfg);

	void acquireData(void);

	bool retrieveData(uint32_t* size, void* data);
	bool retrieveData(void* opt, uint32_t* size, void* data);

private:
	char readRegister(uint32_t address);
	void writeRegister(uint32_t address, char data);
	char readRegisterMask(uint32_t address, char mask);
	void writeRegisterMask(uint32_t address, char mask, char data);
	char readMagRegister(char address);
	void writeMagRegister(char address, char data);
	void writeMagRegisterMask(uint32_t address, char mask, char data);
	void readMagRegisterSet(uint32_t startAddress, char readCount, char* pOut);
	void applyCalibration(void);
};



#endif /* SENSE_SPNNINEAXISMOTION_H_ */
