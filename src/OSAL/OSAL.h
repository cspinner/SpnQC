/*
 * OSAL.h
 *
 *  Created on: Sep 27, 2015
 *      Author: cspinner
 */

#ifndef OSAL_OSAL_H_
#define OSAL_OSAL_H_

#include <stdint.h>

#define WORKING_DIRECTORY "/home/pi/Projects/SpnQC/"
#define OSAL_SOCKET_ANYADDR_D 0

typedef struct
{
	uint32_t seconds;
	uint32_t microSeconds;
} OSAL_Time_Type;

typedef enum
{
	OSAL_FILE_CREATE_E,
	OSAL_FILE_APPEND_E
} OSAL_Output_File_Type;

typedef enum
{
	OSAL_SOCKET_STREAM_E
} OSAL_Socket_Type;

bool OSAL_INIT(void);

bool OSAL_TIME_ADD(OSAL_Time_Type* pA, OSAL_Time_Type* pB, OSAL_Time_Type* pResult);
bool OSAL_TIME_SUB(OSAL_Time_Type* pA, OSAL_Time_Type* pB, OSAL_Time_Type* pResult);
bool OSAL_TIME_OF_DAY(OSAL_Time_Type* pTimeOfDay);
bool OSAL_TIME_INT_SET(uint32_t intervalUsec);
bool OSAL_TIME_INT_START(void);
bool OSAL_TIME_INT_STOP(void);

bool OSAL_HANDLE_INIT(void);
bool OSAL_HANDLE_SET_ITMR_CB(void (*pCb)(void));

bool OSAL_INPUT_KB_INIT(void);
bool OSAL_INPUT_KB_GET_HIT(char* pResult);
bool OSAL_INPUT_KB_RESTORE(void);
bool OSAL_INPUT_FILE_OPEN(uint32_t* pFileId, const char *pPathname);
bool OSAL_INPUT_FILE_READ_LINE(uint32_t fileId, char* pDest, uint32_t destSizeBytes, uint32_t* pBytesRead);
bool OSAL_INPUT_FILE_CLOSE(uint32_t fileId);

bool OSAL_OUTPUT_FILE_OPEN(uint32_t* pFileId, const char *pPath, OSAL_Output_File_Type type);
bool OSAL_OUTPUT_FILE_WRITE(uint32_t fileId, const char *pBuf);
bool OSAL_OUTPUT_FILE_CLOSE(uint32_t fileId);

bool OSAL_SOCKET_CREATE(int32_t* pSockFd, OSAL_Socket_Type sockType);
bool OSAL_SOCKET_BIND(int32_t sockFd, uint32_t inAddr, uint16_t port);
bool OSAL_SOCKET_LISTEN(int32_t sockFd, int32_t queueCount);
bool OSAL_SOCKET_ACCEPT(int32_t* pNewSockFd, int32_t sockFd);
bool OSAL_SOCKET_READ(int32_t sockFd, char* pBuf, uint32_t maxReadBytes, uint32_t* pBytesRead);
bool OSAL_SOCKET_WRITE(int32_t sockFd, char* pBuf, uint32_t sizeBytes, uint32_t* pBytesWritten);
bool OSAL_SOCKET_CLOSE(int32_t sockFd);

#endif /* OSAL_OSAL_H_ */
