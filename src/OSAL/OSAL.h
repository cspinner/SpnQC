/*
 * OSAL.h
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */

#ifndef OSAL_OSAL_H_
#define OSAL_OSAL_H_

#include <stdint.h>

typedef struct
{
	uint32_t seconds;
	uint32_t microSeconds;
} OSAL_Time_Type;

bool OSAL_INIT(void);

#endif /* OSAL_OSAL_H_ */
