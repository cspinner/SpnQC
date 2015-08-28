/*
 * spnConfig.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: cspinner
 */

#include "SpnQC.h"
#include <string.h>

typedef enum
{
	UBYTE_E,
	BYTE_E,
	USHORT_E,
	SHORT_E,
	UINT_E,
	INT_E,
	FLOAT_E
} DATATYPE;

typedef struct
{
	const char* textName;
	DATATYPE type;
	void* pDest;
} SpnQC_Config_Entry_Type;

static SpnQC_Config_Type spnQcConfig;

const SpnQC_Config_Entry_Type configEntries[] = {
	{
		"SPICHSEL",
		UINT_E,
		&spnQcConfig.spi.chipSelect
	},
	{
		"SPISPEED",
		UINT_E,
		&spnQcConfig.spi.speed
	},
	{
		"AVGSIZE",
		UINT_E,
		&spnQcConfig.mpu9250.rollingAvgCount
	},
	{
		"MAXSMPDIFF",
		FLOAT_E,
		&spnQcConfig.mpu9250.magOutlierThresh
	},
	{
		"MADGBETA",
		FLOAT_E,
		&spnQcConfig.mpu9250.beta
	},
	{
		"ACCFSSEL",
		UINT_E,
		&spnQcConfig.mpu9250.accFsSel
	},
	{
		"GYROFSSEL",
		UINT_E,
		&spnQcConfig.mpu9250.gyroFsSel
	},
	{
		"ACCXBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.accel.x_bias
	},
	{
		"ACCYBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.accel.y_bias
	},
	{
		"ACCZBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.accel.z_bias
	},
	{
		"GYROXBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.gyro.x_bias
	},
	{
		"GYROYBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.gyro.y_bias
	},
	{
		"GYROZBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.gyro.z_bias
	},
	{
		"MAGXBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.mag.x_bias
	},
	{
		"MAGYBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.mag.y_bias
	},
	{
		"MAGZBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.mag.z_bias
	},
	{
		"MAGXGAIN",
		FLOAT_E,
		&spnQcConfig.mpu9250.mag.x_scale
	},
	{
		"MAGYGAIN",
		FLOAT_E,
		&spnQcConfig.mpu9250.mag.y_scale
	},
	{
		"MAGZGAIN",
		FLOAT_E,
		&spnQcConfig.mpu9250.mag.z_scale
	},
	{
		"CHANCOUNT",
		UINT_E,
		&spnQcConfig.transceiver.chanCount
	},
	{	
        "TPIN1",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[0]
    },	
    {	
        "TPIN2",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[1]
    },	
    {	
        "TPIN3",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[2]
    },	
    {	
        "TPIN4",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[3]
    },	
    {	
        "TPIN5",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[4]
    },	
    {	
        "TPIN6",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[5]
    },	
    {	
        "TPIN7",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[6]
    },	
    {	
        "TPIN8",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[7]
    },	
    {	
        "TERMINPUT",
        INT_E,
        &spnQcConfig.transceiver.useTerminal
    },	
	{
		"MOTCOUNT",
		UINT_E,
		&spnQcConfig.motor.chanCount
	},
	{	
        "MPIN1",
        UINT_E,
        &spnQcConfig.motor.gpioPin[0]
    },	
    {	
        "MPIN2",
        UINT_E,
        &spnQcConfig.motor.gpioPin[1]
    },	
    {	
        "MPIN3",
        UINT_E,
        &spnQcConfig.motor.gpioPin[2]
    },	
    {	
        "MPIN4",
        UINT_E,
        &spnQcConfig.motor.gpioPin[3]
    },	
    {	
        "MPIN5",
        UINT_E,
        &spnQcConfig.motor.gpioPin[4]
    },	
    {	
        "MPIN6",
        UINT_E,
        &spnQcConfig.motor.gpioPin[5]
    },	
    {	
        "MPIN7",
        UINT_E,
        &spnQcConfig.motor.gpioPin[6]
    },	
    {	
        "MPIN8",
        UINT_E,
        &spnQcConfig.motor.gpioPin[7]
    },
    {	
        "PWZEROTH",
        FLOAT_E,
        &spnQcConfig.motor.pulseWidthZeroThrottle
    },
    {	
        "PWFULLTH",
        FLOAT_E,
        &spnQcConfig.motor.pulseWidthFullThrottle
    },
	{
		"PIDOUTMIN",
		FLOAT_E,
		&spnQcConfig.command.pidOutMin
	},
	{
		"PIDOUTMAX",
		FLOAT_E,
		&spnQcConfig.command.pidOutMax
	},
	{
		"PITCHKP",
		FLOAT_E,
		&spnQcConfig.command.pidPitchKp
	},
	{
		"PITCHKI",
		FLOAT_E,
		&spnQcConfig.command.pidPitchKi
	},
	{
		"PITCHKD",
		FLOAT_E,
		&spnQcConfig.command.pidPitchKd
	},
	{
		"ROLLKP",
		FLOAT_E,
		&spnQcConfig.command.pidRollKp
	},
	{
		"ROLLKI",
		FLOAT_E,
		&spnQcConfig.command.pidRollKi
	},
	{
		"ROLLKD",
		FLOAT_E,
		&spnQcConfig.command.pidRollKd
	},
	{
		"YAWKP",
		FLOAT_E,
		&spnQcConfig.command.pidYawKp
	},
	{
		"YAWKI",
		FLOAT_E,
		&spnQcConfig.command.pidYawKi
	},
	{
		"YAWKD",
		FLOAT_E,
		&spnQcConfig.command.pidYawKd
	}
};

const uint32_t configEntriesCount = sizeof(configEntries)/sizeof(SpnQC_Config_Entry_Type);

bool spnConfigInit(void)
{
	bool status = SUCCESS;
	FILE* pInputFile = NULL; // input file descriptor

	//
	// ATTEMPT TO READ CONFIG FILE
	//

	// open the file
	spnUtilsOpenFileForRead(&pInputFile, "SpnQC.cfg");

	// If file exists, parse the data and populate config structure
	if(pInputFile != NULL)
	{
		char buf[128];

		while(spnUtilsReadLine(pInputFile, buf, 128) && (status == SUCCESS))
		{
			char entryNameRead[64];
			char entryValueRead[64];

			if(strchr(buf, '=') != NULL)
			{
				// Separate entry name and value from input string
				sscanf(buf, "%[^=]=%s", entryNameRead, entryValueRead);

				for(uint32_t entry = 0; entry < configEntriesCount; entry++)
				{
					if(0 == strcmp(entryNameRead, configEntries[entry].textName))
					{
						// match found
						// parse and write data to destination memory based on configured type:
						switch(configEntries[entry].type)
						{
							case UBYTE_E:
								sscanf(entryValueRead, "%hhu", (char*)configEntries[entry].pDest);
								break;

							case BYTE_E:
								sscanf(entryValueRead, "%c", (char*)configEntries[entry].pDest);
								break;

							case USHORT_E:
								sscanf(entryValueRead, "%hu", (uint16_t*)configEntries[entry].pDest);
								break;

							case SHORT_E:
								sscanf(entryValueRead, "%hi", (int16_t*)configEntries[entry].pDest);
								break;

							case UINT_E:
								sscanf(entryValueRead, "%u", (uint32_t*)configEntries[entry].pDest);
								break;

							case INT_E:
								sscanf(entryValueRead, "%i", (int32_t*)configEntries[entry].pDest);
								break;

							case FLOAT_E:
								sscanf(entryValueRead, "%f", (float32_t*)configEntries[entry].pDest);
								break;

							default:
								printf("Program Error: Invalid config type.\n");
								status = FAIL;
								break;
						}

						break; // break the for loop after match was found
					}
				}
			}

			// Reinitialize buffer
			memset(buf, 0, sizeof(buf));
		}

		// close the file
		spnUtilsCloseFile(pInputFile);
	}
	else
	{
		perror("Failed to open SpnQC.cfg.");
		status = FAIL;
	}

	return status;
}

const SpnQC_Config_Type* const spnConfigGet(void)
{
	return (const SpnQC_Config_Type* const)&spnQcConfig;
}
