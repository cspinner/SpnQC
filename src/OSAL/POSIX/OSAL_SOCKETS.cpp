/*
 * OSAL_SOCKETS.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: cspinner
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <err.h>
#include <fcntl.h>
#include "OSAL.h"

bool OSAL_SOCKET_CREATE(int32_t* pSockFd, OSAL_Socket_Type sockType)
{
	int32_t sockfd;

	// Stream is the only supported socket at this time
	if(sockType == OSAL_SOCKET_STREAM_E)
	{
		// create socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0);

		if (sockfd < 0)
		{
			perror("ERROR opening socket\n");
			return EXIT_FAILURE;
		}
		else
		{
			*pSockFd = sockfd;

			return EXIT_SUCCESS;
		}
	}
	else
	{
		return EXIT_FAILURE;
	}
}

bool OSAL_SOCKET_BIND(int32_t sockFd, uint32_t inAddr, uint16_t port)
{
	uint16_t portno;
	struct sockaddr_in serv_addr;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = port;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inAddr;
	serv_addr.sin_port = htons(portno);

	// bind socket to local address
	if (bind(sockFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR on binding\n");
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}

bool OSAL_SOCKET_LISTEN(int32_t sockFd, int32_t queueCount)
{
	// listen for clients
	if(listen(sockFd, queueCount) == 0)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

bool OSAL_SOCKET_ACCEPT(int32_t* pNewSockFd, int32_t sockFd)
{
	int32_t newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr;

	clilen = sizeof(cli_addr);

	// accept is a blocking call
	newsockfd = accept(sockFd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
	{
	   perror("ERROR on accept\n");
	   return EXIT_FAILURE;
	}
	else
	{
		// make receives non-blocking
		fcntl(newsockfd, F_SETFL, O_NONBLOCK);
		*pNewSockFd = newsockfd;
		return EXIT_SUCCESS;
	}
}

bool OSAL_SOCKET_READ(int32_t sockFd, char* pBuf, uint32_t maxReadBytes, uint32_t* pBytesRead)
{
	*pBytesRead = read(sockFd, pBuf, maxReadBytes);

	return EXIT_SUCCESS;
}

bool OSAL_SOCKET_WRITE(int32_t sockFd, char* pBuf, uint32_t sizeBytes, uint32_t* pBytesWritten)
{
	*pBytesWritten = write(sockFd, pBuf, sizeBytes);

	return EXIT_SUCCESS;
}

bool OSAL_SOCKET_CLOSE(int32_t sockFd)
{
	close(sockFd);

	return EXIT_SUCCESS;
}
