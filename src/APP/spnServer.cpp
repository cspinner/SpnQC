/*
 * spnServer.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: cspinner
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "spnQC.h"
#include "OSAL.h"

static int32_t sockfd = -1;
static int32_t newsockfd = -1;

static void serverOnExit(void);

bool spnServerInit(uint16_t port)
{
	// create socket
	if(OSAL_SOCKET_CREATE(&sockfd, OSAL_SOCKET_STREAM_E) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	else
	{
		atexit(&serverOnExit);

		// bind socket to any incoming address on specified port,
		// listen for clients
	    if((OSAL_SOCKET_BIND(sockfd, OSAL_SOCKET_ANYADDR_D, port) == EXIT_FAILURE) ||
		   (OSAL_SOCKET_LISTEN(sockfd, 5) == EXIT_FAILURE))
		{
			return EXIT_FAILURE;
		}
		else
		{
			return EXIT_SUCCESS;
		}
	}
}

void spnServerWaitForGroundStation(void)
{
	// accept is a blocking call
	if(OSAL_SOCKET_ACCEPT(&newsockfd, sockfd) == EXIT_FAILURE)
	{
		exit(EXIT_FAILURE);
	}
}

uint32_t spnServerReadMessage(char* buf, uint32_t maxReadBytes)
{
	uint32_t bytesRead;
	memset(buf, 0, maxReadBytes);

	OSAL_SOCKET_READ(newsockfd, buf, maxReadBytes, &bytesRead);

	return bytesRead;
}

void spnServerWriteMessage(char* buf, uint32_t sizeBytes)
{
	uint32_t bytesWritten;

	OSAL_SOCKET_WRITE(newsockfd, buf, sizeBytes, &bytesWritten);
}

static void serverOnExit(void)
{
	printf("Closing sockets...\n");

	if(sockfd != -1) OSAL_SOCKET_CLOSE(sockfd);
	if(newsockfd != -1) OSAL_SOCKET_CLOSE(newsockfd);
}

