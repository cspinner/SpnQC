/*
 * spnNineAxisMotion.h
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#ifndef SENSE_SPNNINEAXISMOTION_H_
#define SENSE_SPNNINEAXISMOTION_H_

#include "spnSensor.h"

enum
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
	NUM_AXIS,
};

typedef struct
{
	// These biases will be added to input data
	float32_t accel[NUM_AXIS]; // g
	float32_t gyro[NUM_AXIS]; // º/s
	float32_t magb[NUM_AXIS]; // mG
	float32_t mags[NUM_AXIS]; // scale
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
	float32_t accel[NUM_AXIS]; // g
	float32_t gyro[NUM_AXIS]; // º/s
	float32_t mag[NUM_AXIS]; // mG
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
