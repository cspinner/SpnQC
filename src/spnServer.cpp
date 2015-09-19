/*
 * spnServer.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: cspinner
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <err.h>
#include <fcntl.h>
#include "spnQC.h"

static int32_t sockfd = -1;
static int32_t newsockfd = -1;

static void serverOnExit(void);

bool spnServerInit(uint16_t port)
{
	uint16_t portno;
	struct sockaddr_in serv_addr;

	// create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror("ERROR opening socket\n");
		return EXIT_FAILURE;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = port;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// bind socket to local address
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR on binding\n");
		return EXIT_FAILURE;
	}

	// listen for clients
	listen(sockfd,5);

	atexit(&serverOnExit);

	return EXIT_SUCCESS;
}

void spnServerWaitForGroundStation(void)
{
	socklen_t clilen;
	struct sockaddr_in cli_addr;

	clilen = sizeof(cli_addr);

	// accept is a blocking call
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
	{
	   perror("ERROR on accept\n");
	   exit(EXIT_FAILURE);
	}

	// make receives non-blocking
	fcntl(newsockfd, F_SETFL, O_NONBLOCK);
}

uint32_t spnServerReadMessage(char* buf, uint32_t maxReadBytes)
{
	bzero(buf, maxReadBytes);

	return read(newsockfd, buf, maxReadBytes);
}

void spnServerWriteMessage(char* buf, uint32_t sizeBytes)
{
	write(newsockfd, buf, sizeBytes);
}

static void serverOnExit(void)
{
	printf("Closing sockets...\n");
	if(sockfd != -1) close(sockfd);
	if(newsockfd != -1) close(newsockfd);
}

