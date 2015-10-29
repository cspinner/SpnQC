/*
 * spnConfig.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: cspinner
 */

#include "spnQC.h"
#include <string.h>
#include <stdlib.h>

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
		&spnQcConfig.mpu9250.accel[0]
	},
	{
		"ACCYBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.accel[1]
	},
	{
		"ACCZBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.accel[2]
	},
	{
		"GYROXBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.gyro[0]
	},
	{
		"GYROYBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.gyro[1]
	},
	{
		"GYROZBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.gyro[2]
	},
	{
		"MAGXBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.magb[0]
	},
	{
		"MAGYBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.magb[1]
	},
	{
		"MAGZBIAS",
		FLOAT_E,
		&spnQcConfig.mpu9250.magb[2]
	},
	{
		"MAGXGAIN",
		FLOAT_E,
		&spnQcConfig.mpu9250.mags[0]
	},
	{
		"MAGYGAIN",
		FLOAT_E,
		&spnQcConfig.mpu9250.mags[1]
	},
	{
		"MAGZGAIN",
		FLOAT_E,
		&spnQcConfig.mpu9250.mags[2]
	},
	{
		"CHANCOUNT",
		UINT_E,
		&spnQcConfig.transceiver.chanCount
	},
	{	
        "THROTTLEPIN",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[0]
    },	
    {	
        "ELEVATORPIN",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[1]
    },	
    {	
        "AILERONPIN",
        UINT_E,
        &spnQcConfig.transceiver.gpioPin[2]
    },	
    {	
        "RUDDERPIN",
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
        "TRANINPUT",
        INT_E,
        &spnQcConfig.transceiver.useTransmitter
    },
    {
        "PWZERO",
        INT_E,
        &spnQcConfig.transceiver.pulseWidthZero
    },
    {
        "PWFULL",
        INT_E,
        &spnQcConfig.transceiver.pulseWidthFull
    },
    {
        "MAXTHRNG",
        INT_E,
        &spnQcConfig.transceiver.maxThrottleRngPct
    },
    {
        "MAXELERNG",
        INT_E,
        &spnQcConfig.transceiver.maxElevatorRngDeg
    },
    {
        "MAXAILRNG",
        INT_E,
        &spnQcConfig.transceiver.maxAileronRngDeg
    },
    {
        "MAXRUDRNG",
        INT_E,
        &spnQcConfig.transceiver.maxRudderRngDeg
    },
    {
        "NETINPUT",
        INT_E,
        &spnQcConfig.transceiver.useNetworkInput
    },
    {
        "NETOUTPUT",
        INT_E,
        &spnQcConfig.transceiver.outputToNetwork
    },
    {
        "NETPORT",
        USHORT_E,
        &spnQcConfig.transceiver.netPort
    },
    {
        "NETHBINT",
        INT_E,
        &spnQcConfig.transceiver.netHeartbeatInterval.seconds
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
        "M1ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[0]
    },
    {
        "M2ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[1]
    },
    {
        "M3ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[2]
    },
    {
        "M4ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[3]
    },
    {
        "M5ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[4]
    },
    {
        "M6ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[5]
    },
    {
        "M7ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[6]
    },
    {
        "M8ENABLE",
        INT_E,
        &spnQcConfig.motor.motorEnable[7]
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
	PRINT("start spnConfigInit");

	bool status = EXIT_SUCCESS;
	uint32_t InputFile; // input file descriptor

	//
	// ATTEMPT TO READ CONFIG FILE
	//

	// open the file
	// If file exists, parse the data and populate config structure
	if(spnUtilsOpenFileForRead(&InputFile, "SpnQC.cfg") == EXIT_SUCCESS)
	{
		char buf[128];

		while((spnUtilsReadLine(InputFile, buf, 128) > 0) && (status == EXIT_SUCCESS))
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
								status = EXIT_FAILURE;
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
		spnUtilsCloseInputFile(InputFile);
	}
	else
	{
		perror("Failed to open SpnQC.cfg.");
		status = EXIT_FAILURE;
	}

	return status;
}

const SpnQC_Config_Type* const spnConfigGet(void)
{
	return (const SpnQC_Config_Type* const)&spnQcConfig;
}
