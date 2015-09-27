/*
 * spnNineAxisMotion.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include "spnSensor.h"
#include "spnNineAxisMotion.h"
#include "wiringPiSPI.h"
#include "spnFilter.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define MPU9250_ROOM_TEMP_OFFSET 21.0f // °C
#define MPU9250_TEMP_SENSITIVITY 333.87f // LSB/°C
#define MPU9250_IDENTIFIER 0x71u // Expected value of WHOAMI
#define AK8963_IDENTIFIER 0x48u // Expected value of WIA

typedef enum
{
	WRITE = 0x00,
	READ = 0x80
} Transfer_Type;

typedef enum
{
	MPU9250_CONFIG_ADDR = 0x1A,
	MPU9250_GYRO_CONFIG_ADDR = 0x1B,
	MPU9250_ACCEL_CONFIG_ADDR = 0x1C,
	MPU9250_ACCEL_CONFIG2_ADDR = 0x1D,
	MPU9250_GYRO_X_OFFS_USR_H_ADDR = 0x13,
	MPU9250_GYRO_X_OFFS_USR_L_ADDR = 0x14,
	MPU9250_GYRO_Y_OFFS_USR_H_ADDR = 0x15,
	MPU9250_GYRO_Y_OFFS_USR_L_ADDR = 0x16,
	MPU9250_GYRO_Z_OFFS_USR_H_ADDR = 0x17,
	MPU9250_GYRO_Z_OFFS_USR_L_ADDR = 0x18,
	MPU9250_FIFO_EN_ADDR_ADDR = 0x23,
	MPU9250_I2C_MST_CTRL_ADDR = 0x24,
	MPU9250_I2C_SLV0_ADDR_ADDR = 0x25,
	MPU9250_I2C_SLV0_REG_ADDR = 0x26,
	MPU9250_I2C_SLV0_CTRL_ADDR = 0x27,
	MPU9250_I2C_MST_STATUS_ADDR = 0x36,
	MPU9250_ACCEL_XOUT_H_ADDR = 0x3B,
	MPU9250_ACCEL_XOUT_L_ADDR = 0x3C,
	MPU9250_ACCEL_YOUT_H_ADDR = 0x3D,
	MPU9250_ACCEL_YOUT_L_ADDR = 0x3E,
	MPU9250_ACCEL_ZOUT_H_ADDR = 0x3F,
	MPU9250_ACCEL_ZOUT_L_ADDR = 0x40,
	MPU9250_TEMP_OUT_H_ADDR = 0x41,
	MPU9250_TEMP_OUT_L_ADDR = 0x42,
	MPU9250_GYRO_XOUT_H_ADDR = 0x43,
	MPU9250_GYRO_XOUT_L_ADDR = 0x44,
	MPU9250_GYRO_YOUT_H_ADDR = 0x45,
	MPU9250_GYRO_YOUT_L_ADDR = 0x46,
	MPU9250_GYRO_ZOUT_H_ADDR = 0x47,
	MPU9250_GYRO_ZOUT_L_ADDR = 0x48,
	MPU9250_EXT_SENS_DATA_BASE_ADDR = 0x49, // 0x49 - 0x60
	MPU9250_I2C_SLV0_DO_ADDR = 0x63,
	MPU9250_SIGNAL_PATH_RESET_ADDR = 0x68,
	MPU9250_USER_CONTROL_ADDR = 0x6A,
	MPU9250_PWR_MGMT_1_ADDR = 0x6B,
	MPU9250_PWR_MGMT_2_ADDR = 0x6C,
	MPU9250_WHO_AM_I_ADDR = 0x75,
	MPU9250_ACCEL_X_OFFS_USR_H_ADDR = 0x77,
	MPU9250_ACCEL_X_OFFS_USR_L_ADDR = 0x78,
	MPU9250_ACCEL_Y_OFFS_USR_H_ADDR = 0x7A,
	MPU9250_ACCEL_Y_OFFS_USR_L_ADDR = 0x7B,
	MPU9250_ACCEL_Z_OFFS_USR_H_ADDR = 0x7D,
	MPU9250_ACCEL_Z_OFFS_USR_L_ADDR = 0x7E
} MPU9250_Reg_Addr_Type;

typedef enum
{
	AK8963_WIA_ADDR = 0x00,
	AK8963_INFO_ADDR = 0x01,
	AK8963_ST1_ADDR = 0x02,
	AK8963_HXL_ADDR = 0x03,
	AK8963_HXH_ADDR = 0x04,
	AK8963_HYL_ADDR = 0x05,
	AK8963_HYH_ADDR = 0x06,
	AK8963_HZL_ADDR = 0x07,
	AK8963_HZH_ADDR = 0x08,
	AK8963_ST2_ADDR = 0x09,
	AK8963_CNTL1_ADDR = 0x0A,
	AK8963_CNTL2_ADDR = 0x0B,
	AK8963_ASTC_ADDR = 0x0C,
	AK8963_TS1_ADDR = 0x0D,
	AK8963_TS2_ADDR = 0x0E,
	AK8963_I2CDIS_ADDR = 0x0F,
	AK8963_ASAX_ADDR = 0x10,
	AK8963_ASAY_ADDR = 0x11,
	AK8963_ASAZ_ADDR = 0x12,
	AK8963_RSV_ADDR = 0x13
} AK8963;

// LSB/(º/s) - indexed by FS_SEL
const float32_t GYRO_SENSITIVITY[4] =
{
	131.0, 65.5, 32.8, 16.4
};

// LSB/g - indexed by FS_SEL
const float32_t ACCEL_SENSITIVITY[4] =
{
	16384.0, 8192.0, 4096.0, 2048.0
};

char SpnNineAxisMotion::readRegister(uint32_t address)
{
	char spiTransfer[2];

	spiTransfer[0] = address;
	spiTransfer[0] |= READ;
	spiTransfer[1] = 0x0;
	wiringPiSPIDataRW(spi_fd, (uint8_t*)&spiTransfer, sizeof(spiTransfer));

	return spiTransfer[1];
}

void SpnNineAxisMotion::writeRegister(uint32_t address, char data)
{
	char spiTransfer[2];

	spiTransfer[0] = address;
	spiTransfer[0] |= WRITE;
	spiTransfer[1] = data;
	wiringPiSPIDataRW(spi_fd, (uint8_t*)&spiTransfer, sizeof(spiTransfer));
}

char SpnNineAxisMotion::readRegisterMask(uint32_t address, char mask)
{
	char maskIn = mask;
	char valueRead = readRegister(address) & mask;

	while(0 == (maskIn & 0x01))             /* Shift the value to the left until    */
	{                                       /* it aligns with the bit field         */
		maskIn = maskIn >> 1;
		valueRead = valueRead >> 1;
	}

	return valueRead;
}

void SpnNineAxisMotion::writeRegisterMask(uint32_t address, char mask, char data)
{
	char maskIn = mask;
	char writeData = data;
	char readData = readRegister(address);

	while(0 == (maskIn & 0x01))             /* Shift the value to the left until    */
	{                                       /* it aligns with the bit field         */
		maskIn = maskIn >> 1;
		writeData = writeData << 1;
	}

	writeRegister(address, ((readData & ~mask) | writeData));
}

void SpnNineAxisMotion::readMagRegisterSet(uint32_t startAddress, char readCount, char* pOut)
{
	uint32_t i;

	writeRegister(MPU9250_I2C_SLV0_ADDR_ADDR, 0x8C); // Read from I2C address 12
	writeRegister(MPU9250_I2C_SLV0_REG_ADDR, startAddress); // Reg address
	writeRegister(MPU9250_I2C_SLV0_CTRL_ADDR, 0x80 | (readCount&0xF)); // Enable SLV0 and transfer readCount bytes

	// Wait for data to be written to EXT_SENS_DATA register
	spnUtilsWaitUsec(1000);

	for(i = 0; i < (readCount&0xF); i++)
	{
		pOut[i] = readRegister(MPU9250_EXT_SENS_DATA_BASE_ADDR+i);
	}
}

void SpnNineAxisMotion::writeMagRegister(char address, char data)
{
	writeRegister(MPU9250_I2C_SLV0_ADDR_ADDR, 0x0C); // Write to I2C address 12
	writeRegister(MPU9250_I2C_SLV0_REG_ADDR, address); // Reg address
	writeRegister(MPU9250_I2C_SLV0_DO_ADDR, data);
	writeRegister(MPU9250_I2C_SLV0_CTRL_ADDR, 0x81); // Enable SLV0, write 1 byte

	// Allow write to complete
	spnUtilsWaitUsec(1000);
}

void SpnNineAxisMotion::writeMagRegisterMask(uint32_t address, char mask, char data)
{
	char maskIn = mask;
	char writeData = data;
	char readData = mag_registers.regByIndex[address];

	while(0 == (maskIn & 0x01))             /* Shift the value to the left until    */
	{                                       /* it aligns with the bit field         */
		maskIn = maskIn >> 1;
		writeData = writeData << 1;
	}

	writeMagRegister(address, ((readData & ~mask) | writeData));
}

// For the MPU9250 sensor
SpnNineAxisMotion::SpnNineAxisMotion(void)
:SpnSensor()
{
	spi_fd = 0;
	chipSelect = 0;
	speed = 0;
	accFsSel = 0;
	gyroFsSel = 0;
	acquireCount = 0;
	magOutlierThresh = 0;
	temperatureData = 0;
}

bool SpnNineAxisMotion::configure(void* cfg)
{
	SpnNineAxisMotion_Cfg_Type* mpu9250_config = (SpnNineAxisMotion_Cfg_Type*)cfg;
	bool status = EXIT_SUCCESS;

	if(SpnSensor::configure(cfg) == EXIT_SUCCESS)
	{
		// SPI
		if(spi_fd == 0) // Protects against multiple calls to "configure"
		{
			// Configure the SPI Bus
			chipSelect = mpu9250_config->chipSelect;
			speed = mpu9250_config->speed;
			spi_fd = wiringPiSPISetup(chipSelect, speed);
		}

//		accelFiltWindow = mpu9250_config->accelFilterWindow;
//		gyroFiltWindow = mpu9250_config->gyroFilterWindow;
//		magFiltWindow = mpu9250_config->magFilterWindow;
		magOutlierThresh = mpu9250_config->magOutlierThresh;
		accFsSel = mpu9250_config->accFsSel;
		gyroFsSel = mpu9250_config->gyroFsSel;
		calData = mpu9250_config->calibration;
		acquireCount = 0;

		for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
		{
			accelDataFilter[axis].configure(FILT_LP_2P_7HZBR_200HZSMP);
			gyroDataFilter[axis].configure(FILT_LP_2P_7HZBR_200HZSMP);
			magDataFilter[axis].configure(FILT_LP_2P_7HZBR_200HZSMP);
		}

		// First reset the MPU9250
		writeRegisterMask(MPU9250_PWR_MGMT_1_ADDR, 0x80, 0x1);

		// Wait for reset to complete
		spnUtilsWaitUsec(1000);

		writeRegister(MPU9250_PWR_MGMT_1_ADDR, 0x01); // auto select best clock
		writeRegister(MPU9250_PWR_MGMT_2_ADDR, 0x00); // all sub components enabled

		writeRegister(MPU9250_USER_CONTROL_ADDR, 0x3); // Reset FIFO, I2C, and Signal Paths

		// Validate device ID
		char deviceId = readRegister(MPU9250_WHO_AM_I_ADDR);
		if(deviceId != MPU9250_IDENTIFIER)
		{
			printf("MPU9250 Identification Check Failed.\n");
			printf("Read: 0x%X, Expected: 0x%X\n", deviceId, MPU9250_IDENTIFIER);
			status = EXIT_FAILURE;
		}
		else
		{
			// configure gyro and accel
			writeRegisterMask(MPU9250_GYRO_CONFIG_ADDR, 0x18, gyroFsSel);
			writeRegisterMask(MPU9250_GYRO_CONFIG_ADDR, 0x03, 0x3); // fchoice_b = 2'b11
			writeRegisterMask(MPU9250_ACCEL_CONFIG_ADDR, 0x18, accFsSel);
			writeRegisterMask(MPU9250_ACCEL_CONFIG2_ADDR, 0x08, 0x1); // accel_fchoice_b = 2'b1

			//
			// Apply GYRO user offset
			//
			int16_t gyroOffsetX, gyroOffsetY, gyroOffsetZ;
			float32_t gyroSensitivity = GYRO_SENSITIVITY[gyroFsSel];

			// Convert offset into format required by the register. Divide by 4 to get 32.9 LSB per deg/s
			//  to conform to expected bias input format
			gyroOffsetX = (int16_t)((calData.gyro[X_AXIS] * gyroSensitivity * pow(2, gyroFsSel)) / 4.0);
			gyroOffsetY = (int16_t)((calData.gyro[Y_AXIS] * gyroSensitivity * pow(2, gyroFsSel)) / 4.0);
			gyroOffsetZ = (int16_t)((calData.gyro[Z_AXIS] * gyroSensitivity * pow(2, gyroFsSel)) / 4.0);

			writeRegisterMask(MPU9250_GYRO_X_OFFS_USR_H_ADDR, 0xFFu, ((gyroOffsetX>>8) & 0xFFu));
			writeRegisterMask(MPU9250_GYRO_X_OFFS_USR_L_ADDR, 0xFFu, (gyroOffsetX & 0xFFu));
			writeRegisterMask(MPU9250_GYRO_Y_OFFS_USR_H_ADDR, 0xFFu, ((gyroOffsetY>>8) & 0xFFu));
			writeRegisterMask(MPU9250_GYRO_Y_OFFS_USR_L_ADDR, 0xFFu, (gyroOffsetY & 0xFFu));
			writeRegisterMask(MPU9250_GYRO_Z_OFFS_USR_H_ADDR, 0xFFu, ((gyroOffsetZ>>8) & 0xFFu));
			writeRegisterMask(MPU9250_GYRO_Z_OFFS_USR_L_ADDR, 0xFFu, (gyroOffsetZ & 0xFFu));

			//
			// Apply ACCEL user offset
			//
			int16_t accelOffsetRegX, accelOffsetRegY, accelOffsetRegZ;
			int16_t accelOffsetX, accelOffsetY, accelOffsetZ;
			float32_t accelSensitivity = ACCEL_SENSITIVITY[accFsSel];

			// Apparently there is factory bias already loaded into the accel registers.
			//  We will need to add our bias to it.
			accelOffsetRegX = (readRegister(MPU9250_ACCEL_X_OFFS_USR_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_X_OFFS_USR_L_ADDR);
			accelOffsetRegY = (readRegister(MPU9250_ACCEL_Y_OFFS_USR_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_Y_OFFS_USR_L_ADDR);
			accelOffsetRegZ = (readRegister(MPU9250_ACCEL_Z_OFFS_USR_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_Z_OFFS_USR_L_ADDR);

			// Convert offset into format required by the register. Divide by 8 to get 2048 LSB per g
			//  to conform to expected bias input format. Mask LSB since it must be preserved.
			accelOffsetX = accelOffsetRegX + ((int16_t)(calData.accel[X_AXIS] * accelSensitivity * pow(2, accFsSel) / 8.0) & ~1);
			accelOffsetY = accelOffsetRegY + ((int16_t)(calData.accel[Y_AXIS] * accelSensitivity * pow(2, accFsSel) / 8.0) & ~1);
			accelOffsetZ = accelOffsetRegZ + ((int16_t)(calData.accel[Z_AXIS] * accelSensitivity * pow(2, accFsSel) / 8.0) & ~1);

			writeRegister(MPU9250_ACCEL_X_OFFS_USR_H_ADDR, ((accelOffsetX>>8) & 0xFFu));
			writeRegister(MPU9250_ACCEL_X_OFFS_USR_L_ADDR, (accelOffsetX & 0xFFu));
			writeRegister(MPU9250_ACCEL_Y_OFFS_USR_H_ADDR, ((accelOffsetY>>8) & 0xFFu));
			writeRegister(MPU9250_ACCEL_Y_OFFS_USR_L_ADDR, (accelOffsetY & 0xFFu));
			writeRegister(MPU9250_ACCEL_Z_OFFS_USR_H_ADDR, ((accelOffsetZ>>8) & 0xFFu));
			writeRegister(MPU9250_ACCEL_Z_OFFS_USR_L_ADDR, (accelOffsetZ & 0xFFu));

			//
			//MPU-9250’s Magnetometer (AK8963)
			//

			// configure AUX I2C bus
			writeRegister(MPU9250_I2C_MST_CTRL_ADDR, 0x1D); // I2C master clock 400 kHz
			writeRegister(MPU9250_USER_CONTROL_ADDR, 0x20); // Enable I2C master mode

			// First reset the AK8963
			writeMagRegister(AK8963_CNTL2_ADDR, 0x01);

			// Wait for reset to complete
			spnUtilsWaitUsec(1000);

			// Configure Magnetometer CNTRL
			writeMagRegister(AK8963_CNTL1_ADDR, 0x0F); // FUSE Prom Read mode
			readMagRegisterSet(AK8963_ASAX_ADDR, 3, (char*)&mag_registers.regByIndex[16]); // Read registers ASAX, ASAY, ASAZ
			writeMagRegister(AK8963_CNTL1_ADDR, 0x00); // Enter power down mode
			spnUtilsWaitUsec(100); // Wait the required time before exiting power down
			writeMagRegister(AK8963_CNTL1_ADDR, 0x16); // Continuous Measurement Mode 2 (100Hz), 16-bit mode
		}
	}
	else
	{
		status = EXIT_FAILURE;
	}

	return status;
}

void SpnNineAxisMotion::acquireData(void)
{
	SpnSensor::acquireData();

	//
	// READ SENSOR DATA
	//
	rawAccelData[X_AXIS][acquireCount] = ((int16_t)((readRegister(MPU9250_ACCEL_XOUT_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_XOUT_L_ADDR)))*1.0;
	rawAccelData[Y_AXIS][acquireCount] = ((int16_t)((readRegister(MPU9250_ACCEL_YOUT_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_YOUT_L_ADDR)))*1.0;
	rawAccelData[Z_AXIS][acquireCount] = ((int16_t)((readRegister(MPU9250_ACCEL_ZOUT_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_ZOUT_L_ADDR)))*1.0;

	rawGyroData[X_AXIS][acquireCount] = ((int16_t)((readRegister(MPU9250_GYRO_XOUT_H_ADDR) << 8)|readRegister(MPU9250_GYRO_XOUT_L_ADDR)))*1.0;
	rawGyroData[Y_AXIS][acquireCount] = ((int16_t)((readRegister(MPU9250_GYRO_YOUT_H_ADDR) << 8)|readRegister(MPU9250_GYRO_YOUT_L_ADDR)))*1.0;
	rawGyroData[Z_AXIS][acquireCount] = ((int16_t)((readRegister(MPU9250_GYRO_ZOUT_H_ADDR) << 8)|readRegister(MPU9250_GYRO_ZOUT_L_ADDR)))*1.0;

	temperatureData = ((int16_t)((readRegister(MPU9250_TEMP_OUT_H_ADDR) << 8)|readRegister(MPU9250_TEMP_OUT_L_ADDR)))*1.0;

	acquireCount++;

//	readMagRegisterSet(AK8963_HXL_ADDR, 12, (char*)&mag_registers.regByIndex[3]);
//
//	// perform sensitivity adjustment
//	pMagUnfiltered->x_raw = ((int16_t)((mag_registers.regByName.HXH << 8)|mag_registers.regByName.HXL))*1.0;
//	pMagUnfiltered->y_raw = ((int16_t)((mag_registers.regByName.HYH << 8)|mag_registers.regByName.HYL))*1.0;
//	pMagUnfiltered->z_raw = ((int16_t)((mag_registers.regByName.HZH << 8)|mag_registers.regByName.HZL))*1.0;


//  //
//	// APPLY CALIBRATION
//	//
//
//	// Apply factory sensitivity adjustment
//	filtMagData[X_AXIS] = filtMagData[X_AXIS] * (((mag_registers.regByName.ASAX - 128) * 0.5)/128.0 + 1.0);
//	filtMagData[Y_AXIS] = filtMagData[Y_AXIS] * (((mag_registers.regByName.ASAY - 128) * 0.5)/128.0 + 1.0);
//	filtMagData[Z_AXIS] = filtMagData[Z_AXIS] * (((mag_registers.regByName.ASAZ - 128) * 0.5)/128.0 + 1.0);
//
//	// Apply bias to compensate for hard errors (note bias is in mG, need to convert back to uT):
//	filtMagData[X_AXIS] -= calData.magb[X_AXIS] / 10.0;
//	filtMagData[Y_AXIS] -= calData.magb[Y_AXIS] / 10.0;
//	filtMagData[Z_AXIS] -= calData.magb[Z_AXIS] / 10.0;
//
//	// Apply scale to compensate for soft errors:
//	filtMagData[X_AXIS] *= calData.mags[X_AXIS];
//	filtMagData[Y_AXIS] *= calData.mags[Y_AXIS];
//  filtMagData[Z_AXIS] *= calData.mags[Z_AXIS];
}

bool SpnNineAxisMotion::retrieveData(void* opt, uint32_t* size, void* data)
{
	SpnNineAxisMotion_Data_Type output;
	uint32_t idx = (uint32_t)opt;

	if(SpnSensor::retrieveData(opt, size, data) == EXIT_SUCCESS)
	{
		for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
		{
			// Converts raw accelerometer data into g
			output.accel[axis] = rawAccelData[axis][idx] / ACCEL_SENSITIVITY[accFsSel];

			// Converts raw gyroscope data into º/s
			output.gyro[axis] = rawGyroData[axis][idx] / GYRO_SENSITIVITY[gyroFsSel];

			// Convert uT to mG
			output.mag[axis] = rawMagData[axis][idx] * 10.0;
		}

		*size = sizeof(output);
		*(SpnNineAxisMotion_Data_Type*)data = output;
	}

	return getStatus();
}

bool SpnNineAxisMotion::retrieveData(uint32_t* size, void* data)
{
	SpnNineAxisMotion_Data_Type output;

	if(SpnSensor::retrieveData(size, data) == EXIT_SUCCESS)
	{
		//
		// FILTER THE DATA
		//
		float32_t gyroNewRaw[NUM_AXIS][16];
		float32_t accelNewRaw[NUM_AXIS][16];
		uint32_t newindex;

		// Average every pair of samples
		for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
		{
			newindex = 0;

			for(uint32_t i = 0; i < (acquireCount/2); i++)
			{
				gyroNewRaw[axis][newindex] = (rawGyroData[axis][i*2] + rawGyroData[axis][i*2+1])/2;
				accelNewRaw[axis][newindex] = (rawAccelData[axis][i*2] + rawAccelData[axis][i*2+1])/2;
				newindex++;
			}
		}

		// Just use odd remaining sample as-is
		if(acquireCount%2)
		{
			for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
			{
				gyroNewRaw[axis][newindex] = rawGyroData[axis][acquireCount-1];
				accelNewRaw[axis][newindex] = rawAccelData[axis][acquireCount-1];
			}
		}

		// Run data through the filters
		for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
		{
			filtAccelData[axis] = accelDataFilter[axis].update(accelNewRaw[axis],  (acquireCount/2)+(acquireCount%2));
			filtGyroData[axis] = gyroDataFilter[axis].update(gyroNewRaw[axis], (acquireCount/2)+(acquireCount%2));
//			filtMagData[axis] = magDataFilter[axis].update(magNewRaw[axis], &filtMagData[axis], (acquireCount/2)+(acquireCount%2));
		}

//		// RAW DATA PASSTHROUGH
//		for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
//		{
//			filtAccelData[axis] = rawAccelData[axis][0]; // raw pass through
//			filtGyroData[axis] = rawGyroData[axis][0]; // raw pass through
//		}

		acquireCount = 0;

		//
		// CONVERT TO ENGINEERING UNITS
		//

		// Converts raw temperature data into degrees C, then to degrees F
		output.temperature = ((temperatureData - MPU9250_ROOM_TEMP_OFFSET)/MPU9250_TEMP_SENSITIVITY) + MPU9250_ROOM_TEMP_OFFSET;
		output.temperature = output.temperature*1.8 + 32;

		for(uint32_t axis = X_AXIS; axis < NUM_AXIS; axis++)
		{
			// Converts raw accelerometer data into g
			output.accel[axis] = filtAccelData[axis] / ACCEL_SENSITIVITY[accFsSel];

			// Converts raw gyroscope data into º/s
			output.gyro[axis] = filtGyroData[axis] / GYRO_SENSITIVITY[gyroFsSel];

			// Convert uT to mG
			output.mag[axis] = filtMagData[axis] * 10.0;
		}

		*size = sizeof(output);
		*(SpnNineAxisMotion_Data_Type*)data = output;
	}

	return getStatus();
}


