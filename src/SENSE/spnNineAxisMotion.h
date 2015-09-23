/*
 * spnNineAxisMotion.h
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#ifndef SENSE_SPNNINEAXISMOTION_H_
#define SENSE_SPNNINEAXISMOTION_H_

#include "spnSensor.h"
#include "spnFilter.h"

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

typedef struct
{
	union
	{
		struct
		{
			char WIA;
			char INFO;
			char ST1;
			char HXL;
			char HXH;
			char HYL;
			char HYH;
			char HZL;
			char HZH;
			char ST2;
			char CNTL1;
			char CNTL2;
			char ASTC;
			char TS1;
			char TS2;
			char I2CDIS;
			char ASAX;
			char ASAY;
			char ASAZ;
		} regByName;
		char regByIndex[19];
	};

} Mag_Register_Set_Type;

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

	int32_t spi_fd;
	uint32_t chipSelect;
	uint32_t speed;
	uint32_t accFsSel;
	uint32_t gyroFsSel;

	SpnNineAxisMotion_Calibration_Type calData;

	uint32_t acquireCount;
	float32_t magOutlierThresh;

	float32_t rawAccelData[NUM_AXIS][128]; // max 128 sample window
	float32_t rawGyroData[NUM_AXIS][128]; // max 128 sample window
	float32_t rawMagData[NUM_AXIS][128]; // max 128 sample window
	float32_t filtAccelData[NUM_AXIS];
	float32_t filtGyroData[NUM_AXIS];
	float32_t filtMagData[NUM_AXIS];

	float32_t temperatureData;

	SpnFilter accelDataFilter[NUM_AXIS];
	SpnFilter gyroDataFilter[NUM_AXIS];
	SpnFilter magDataFilter[NUM_AXIS];

	Mag_Register_Set_Type mag_registers;
};



#endif /* SENSE_SPNNINEAXISMOTION_H_ */
