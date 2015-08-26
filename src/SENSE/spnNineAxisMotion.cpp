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
#include <string.h>
#include <stdio.h>
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

typedef struct
{
	union
	{
		struct
		{
			uint8_t WIA;
			uint8_t INFO;
			uint8_t ST1;
			uint8_t HXL;
			uint8_t HXH;
			uint8_t HYL;
			uint8_t HYH;
			uint8_t HZL;
			uint8_t HZH;
			uint8_t ST2;
			uint8_t CNTL1;
			uint8_t CNTL2;
			uint8_t ASTC;
			uint8_t TS1;
			uint8_t TS2;
			uint8_t I2CDIS;
			uint8_t ASAX;
			uint8_t ASAY;
			uint8_t ASAZ;
		} regByName;
		uint8_t regByIndex[19];
	};

} Mag_Register_Set_Type;

typedef struct
{
	struct
	{
		float32_t x_raw;
		float32_t y_raw;
		float32_t z_raw;
	} accel;

	struct
	{
		float32_t x_raw;
		float32_t y_raw;
		float32_t z_raw;
	} gyro;

	struct
	{
		float32_t x_raw;
		float32_t y_raw;
		float32_t z_raw;
	} mag;

	float32_t temperature_raw;
} SpnNineAxisMotion_Raw_Data_Type;

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

int32_t spi_fd;
uint32_t chipSelect;
uint32_t speed;
uint32_t accFsSel;
uint32_t gyroFsSel;

SpnNineAxisMotion_Calibration_Type calData;

uint32_t rollingAvgCount;
uint32_t rawDataCount;
float32_t magOutlierThresh;
SpnNineAxisMotion_Raw_Data_Type rawData[128]; // max 128 sample avg
SpnNineAxisMotion_Raw_Data_Type filtData;

Mag_Register_Set_Type mag_registers;

uint8_t SpnNineAxisMotion::readRegister(uint32_t address)
{
	uint8_t spiTransfer[2];

	spiTransfer[0] = address;
	spiTransfer[0] |= READ;
	spiTransfer[1] = 0x0;
	wiringPiSPIDataRW(spi_fd, (uint8_t*)&spiTransfer, sizeof(spiTransfer));

	return spiTransfer[1];
}

void SpnNineAxisMotion::writeRegister(uint32_t address, uint8_t data)
{
	uint8_t spiTransfer[2];

	spiTransfer[0] = address;
	spiTransfer[0] |= WRITE;
	spiTransfer[1] = data;
	wiringPiSPIDataRW(spi_fd, (uint8_t*)&spiTransfer, sizeof(spiTransfer));
}

uint8_t SpnNineAxisMotion::readRegisterMask(uint32_t address, uint8_t mask)
{
	uint8_t maskIn = mask;
	uint8_t valueRead = readRegister(address) & mask;

	while(0 == (maskIn & 0x01))             /* Shift the value to the left until    */
	{                                       /* it aligns with the bit field         */
		maskIn = maskIn >> 1;
		valueRead = valueRead >> 1;
	}

	return valueRead;
}

void SpnNineAxisMotion::writeRegisterMask(uint32_t address, uint8_t mask, uint8_t data)
{
	uint8_t maskIn = mask;
	uint8_t writeData = data;
	uint8_t readData = readRegister(address);

	while(0 == (maskIn & 0x01))             /* Shift the value to the left until    */
	{                                       /* it aligns with the bit field         */
		maskIn = maskIn >> 1;
		writeData = writeData << 1;
	}

	writeRegister(address, ((readData & ~mask) | writeData));
}

void SpnNineAxisMotion::readMagRegisterSet(uint32_t startAddress, uint8_t readCount, uint8_t* pOut)
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

void SpnNineAxisMotion::writeMagRegister(uint8_t address, uint8_t data)
{
	writeRegister(MPU9250_I2C_SLV0_ADDR_ADDR, 0x0C); // Write to I2C address 12
	writeRegister(MPU9250_I2C_SLV0_REG_ADDR, address); // Reg address
	writeRegister(MPU9250_I2C_SLV0_DO_ADDR, data);
	writeRegister(MPU9250_I2C_SLV0_CTRL_ADDR, 0x81); // Enable SLV0, write 1 byte

	// Allow write to complete
	spnUtilsWaitUsec(1000);
}

void SpnNineAxisMotion::writeMagRegisterMask(uint32_t address, uint8_t mask, uint8_t data)
{
	uint8_t maskIn = mask;
	uint8_t writeData = data;
	uint8_t readData = mag_registers.regByIndex[address];

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
	rollingAvgCount = 0;
	rawDataCount = 0;
	accFsSel = 0;
	gyroFsSel = 0;

	memset(&calData, 0, sizeof(calData));
	memset(&rawData, 0, sizeof(rawData));
	memset(&filtData, 0, sizeof(filtData));
	memset(&mag_registers, 0, sizeof(mag_registers));
}

bool SpnNineAxisMotion::configure(void* cfg)
{
	SpnNineAxisMotion_Cfg_Type* mpu9250_config = (SpnNineAxisMotion_Cfg_Type*)cfg;

	setStatus(SpnSensor::configure(cfg));

	if(getStatus() == SUCCESS)
	{
		// SPI
		if(spi_fd == 0) // Protects against multiple calls to "configure"
		{
			// Configure the SPI Bus
			chipSelect = mpu9250_config->chipSelect;
			speed = mpu9250_config->speed;
			spi_fd = wiringPiSPISetup(chipSelect, speed);
		}

		rollingAvgCount = mpu9250_config->rollingAvgCount;
		magOutlierThresh = mpu9250_config->magOutlierThresh;
		accFsSel = mpu9250_config->accFsSel;
		gyroFsSel = mpu9250_config->gyroFsSel;
		calData = mpu9250_config->calibration;
		rawDataCount = 0;

		// First reset the MPU9250
		writeRegisterMask(MPU9250_PWR_MGMT_1_ADDR, 0x80, 0x1);

		// Wait for reset to complete
		spnUtilsWaitUsec(1000);

		writeRegister(MPU9250_PWR_MGMT_1_ADDR, 0x01); // auto select best clock
		writeRegister(MPU9250_PWR_MGMT_2_ADDR, 0x00); // all sub components enabled

		writeRegister(MPU9250_USER_CONTROL_ADDR, 0x3); // Reset FIFO, I2C, and Signal Paths

		// Validate device ID
		uint8_t deviceId = readRegister(MPU9250_WHO_AM_I_ADDR);
		if(deviceId != MPU9250_IDENTIFIER)
		{
			printf("MPU9250 Identification Check Failed.\n");
			printf("Read: 0x%X, Expected: 0x%X\n", deviceId, MPU9250_IDENTIFIER);
			setStatus(FAIL);
		}
		else
		{
			// configure gyro and accel
			writeRegisterMask(MPU9250_GYRO_CONFIG_ADDR, 0x18, gyroFsSel);
			writeRegisterMask(MPU9250_ACCEL_CONFIG_ADDR, 0x18, accFsSel);
			writeRegisterMask(MPU9250_CONFIG_ADDR, 0x07, 0x6); //DLPF_CFG = 6

			//
			// Apply GYRO user offset
			//
			int16_t gyroOffsetX, gyroOffsetY, gyroOffsetZ;
			float32_t gyroSensitivity = GYRO_SENSITIVITY[gyroFsSel];

			// Convert offset into format required by the register. Divide by 4 to get 32.9 LSB per deg/s
			//  to conform to expected bias input format
			gyroOffsetX = (int16_t)((calData.gyro.x_bias * gyroSensitivity * pow(2, gyroFsSel)) / 4.0);
			gyroOffsetY = (int16_t)((calData.gyro.y_bias * gyroSensitivity * pow(2, gyroFsSel)) / 4.0);
			gyroOffsetZ = (int16_t)((calData.gyro.z_bias * gyroSensitivity * pow(2, gyroFsSel)) / 4.0);

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
			accelOffsetX = accelOffsetRegX + ((int16_t)(calData.accel.x_bias * accelSensitivity * pow(2, accFsSel) / 8.0) & ~1);
			accelOffsetY = accelOffsetRegY + ((int16_t)(calData.accel.y_bias * accelSensitivity * pow(2, accFsSel) / 8.0) & ~1);
			accelOffsetZ = accelOffsetRegZ + ((int16_t)(calData.accel.z_bias * accelSensitivity * pow(2, accFsSel) / 8.0) & ~1);

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
			readMagRegisterSet(AK8963_ASAX_ADDR, 3, (uint8_t*)&mag_registers.regByIndex[16]); // Read registers ASAX, ASAY, ASAZ
			writeMagRegister(AK8963_CNTL1_ADDR, 0x00); // Enter power down mode
			spnUtilsWaitUsec(100); // Wait the required time before exiting power down
			writeMagRegister(AK8963_CNTL1_ADDR, 0x16); // Continuous Measurement Mode 2 (100Hz), 16-bit mode
		}
	}

	return getStatus();
}

void SpnNineAxisMotion::acquireData(void)
{
	SpnSensor::acquireData();

	// Use the 0th entry. Data is always right shifted after the algorithm
	SpnNineAxisMotion_Raw_Data_Type* pUnfiltered = &rawData[0];

	//
	// READ SENSOR DATA
	//
	pUnfiltered->accel.x_raw = ((int16_t)((readRegister(MPU9250_ACCEL_XOUT_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_XOUT_L_ADDR)))*1.0;
	pUnfiltered->accel.y_raw = ((int16_t)((readRegister(MPU9250_ACCEL_YOUT_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_YOUT_L_ADDR)))*1.0;
	pUnfiltered->accel.z_raw = ((int16_t)((readRegister(MPU9250_ACCEL_ZOUT_H_ADDR) << 8)|readRegister(MPU9250_ACCEL_ZOUT_L_ADDR)))*1.0;

	pUnfiltered->gyro.x_raw = ((int16_t)((readRegister(MPU9250_GYRO_XOUT_H_ADDR) << 8)|readRegister(MPU9250_GYRO_XOUT_L_ADDR)))*1.0;
	pUnfiltered->gyro.y_raw = ((int16_t)((readRegister(MPU9250_GYRO_YOUT_H_ADDR) << 8)|readRegister(MPU9250_GYRO_YOUT_L_ADDR)))*1.0;
	pUnfiltered->gyro.z_raw = ((int16_t)((readRegister(MPU9250_GYRO_ZOUT_H_ADDR) << 8)|readRegister(MPU9250_GYRO_ZOUT_L_ADDR)))*1.0;

	readMagRegisterSet(AK8963_HXL_ADDR, 12, (uint8_t*)&mag_registers.regByIndex[3]);

	// perform sensitivity adjustment
	pUnfiltered->mag.x_raw = ((int16_t)((mag_registers.regByName.HXH << 8)|mag_registers.regByName.HXL))*1.0;
	pUnfiltered->mag.y_raw = ((int16_t)((mag_registers.regByName.HYH << 8)|mag_registers.regByName.HYL))*1.0;
	pUnfiltered->mag.z_raw = ((int16_t)((mag_registers.regByName.HZH << 8)|mag_registers.regByName.HZL))*1.0;

	pUnfiltered->temperature_raw = ((int16_t)((readRegister(MPU9250_TEMP_OUT_H_ADDR) << 8)|readRegister(MPU9250_TEMP_OUT_L_ADDR)))*1.0;

	if(rawDataCount < rollingAvgCount) rawDataCount++;

	//
	// APPLY FILTERING
	//

	// clear the buffer
	memset(&filtData, 0, sizeof(filtData));

	// Throw out outliers (maintain previous value)
	if(rawDataCount == rollingAvgCount)
	{
		if(fabsf(pUnfiltered->mag.x_raw - pUnfiltered[1].mag.x_raw) > magOutlierThresh)
			pUnfiltered->mag.x_raw = pUnfiltered[1].mag.x_raw;
		if(fabsf(pUnfiltered->mag.y_raw - pUnfiltered[1].mag.y_raw) > magOutlierThresh)
			pUnfiltered->mag.y_raw = pUnfiltered[1].mag.y_raw;
		if(fabsf(pUnfiltered->mag.z_raw - pUnfiltered[1].mag.z_raw) > magOutlierThresh)
			pUnfiltered->mag.z_raw = pUnfiltered[1].mag.z_raw;
	}

	// sum the raw data
	for(uint32_t i = 0; i < rawDataCount; i++)
	{
		filtData.accel.x_raw += pUnfiltered[i].accel.x_raw;
		filtData.accel.y_raw += pUnfiltered[i].accel.y_raw;
		filtData.accel.z_raw += pUnfiltered[i].accel.z_raw;

		filtData.gyro.x_raw += pUnfiltered[i].gyro.x_raw;
		filtData.gyro.y_raw += pUnfiltered[i].gyro.y_raw;
		filtData.gyro.z_raw += pUnfiltered[i].gyro.z_raw;

		filtData.mag.x_raw += pUnfiltered[i].mag.x_raw;
		filtData.mag.y_raw += pUnfiltered[i].mag.y_raw;
		filtData.mag.z_raw += pUnfiltered[i].mag.z_raw;

		filtData.temperature_raw += pUnfiltered[i].temperature_raw;
	}

	// divide by number of samples to get avg
	filtData.accel.x_raw /= rawDataCount;
	filtData.accel.y_raw /= rawDataCount;
	filtData.accel.z_raw /= rawDataCount;

	filtData.gyro.x_raw /= rawDataCount;
	filtData.gyro.y_raw /= rawDataCount;
	filtData.gyro.z_raw /= rawDataCount;

	filtData.mag.x_raw /= rawDataCount;
	filtData.mag.y_raw /= rawDataCount;
	filtData.mag.z_raw /= rawDataCount;

    filtData.temperature_raw /= rawDataCount;


	// Shift un-filtered data right, pushing the oldest data out
	for(uint32_t i = rollingAvgCount-1; i > 0; i--)
	{
		pUnfiltered[i] = pUnfiltered[i-1];
	}
	memset(&pUnfiltered[0], 0, sizeof(SpnNineAxisMotion_Raw_Data_Type)); // clear for the next entry

	//
	// APPLY CALIBRATION
	//

	// Apply factory sensitivity adjustment
	filtData.mag.x_raw = filtData.mag.x_raw * (((mag_registers.regByName.ASAX - 128) * 0.5)/128.0 + 1.0);
	filtData.mag.y_raw = filtData.mag.y_raw * (((mag_registers.regByName.ASAY - 128) * 0.5)/128.0 + 1.0);
	filtData.mag.z_raw = filtData.mag.z_raw * (((mag_registers.regByName.ASAZ - 128) * 0.5)/128.0 + 1.0);

	// Apply bias to compensate for hard errors (note bias is in mG, need to convert back to uT):
	filtData.mag.x_raw -= calData.mag.x_bias / 10.0;
	filtData.mag.y_raw -= calData.mag.y_bias / 10.0;
	filtData.mag.z_raw -= calData.mag.z_bias / 10.0;

	// Apply scale to compensate for soft errors:
	filtData.mag.x_raw *= calData.mag.x_scale;
	filtData.mag.y_raw *= calData.mag.y_scale;
    filtData.mag.z_raw *= calData.mag.z_scale;
}

bool SpnNineAxisMotion::retrieveData(uint32_t* size, void* data)
{
	SpnNineAxisMotion_Data_Type output;
	setStatus(SpnSensor::retrieveData(size, data));

	if(getStatus() == SUCCESS)
	{
		//
		// CONVERT TO ENGINEERING UNITS
		//

		// Converts raw temperature data into degrees C, then to degrees F
		output.temperature = ((filtData.temperature_raw - MPU9250_ROOM_TEMP_OFFSET)/MPU9250_TEMP_SENSITIVITY) + MPU9250_ROOM_TEMP_OFFSET;
		output.temperature = output.temperature*1.8 + 32;

		// Converts raw accelerometer data into g
		output.accel.x = filtData.accel.x_raw / ACCEL_SENSITIVITY[accFsSel];
		output.accel.y = filtData.accel.y_raw / ACCEL_SENSITIVITY[accFsSel];
		output.accel.z = filtData.accel.z_raw / ACCEL_SENSITIVITY[accFsSel];

		// Converts raw gyroscope data into º/s
		output.gyro.x = filtData.gyro.x_raw / GYRO_SENSITIVITY[gyroFsSel];
		output.gyro.y = filtData.gyro.y_raw / GYRO_SENSITIVITY[gyroFsSel];
		output.gyro.z = filtData.gyro.z_raw / GYRO_SENSITIVITY[gyroFsSel];

		// Convert uT to mG
		output.mag.x = filtData.mag.x_raw * 10.0;
		output.mag.y = filtData.mag.y_raw * 10.0;
		output.mag.z = filtData.mag.z_raw * 10.0;

		*size = sizeof(output);
		*(SpnNineAxisMotion_Data_Type*)data = output;
	}

	return getStatus();
}


