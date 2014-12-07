//
//  main.c
//  tcp_socket
//
//  Created by 小林大志 on 2014/11/06.
//  Copyright (c) 2014年 小林大志. All rights reserved.
//
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "die_with_error.h"

#define MAXPENDING 2
#define RCVBUFSIZE 32

int static sum = 0;

void HandleTCPClient(int clntSocket);

int main(int argc, char *argv[]) {
		int servSock;
		int clntSock;
		struct sockaddr_in echoServAddr;
		struct sockaddr_in echoClntAddr;
		unsigned short echoServPort;
		unsigned int clntLen;

		if (argc != 2) {
				fprintf(stderr, "Usage: %s <Server Port> \n", argv[0]);
		}

		echoServPort = atoi(argv[1]);

		if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
				DieWithError("aaasocket() failed");

		memset(&echoServAddr, 0, sizeof(echoServAddr));
		echoServAddr.sin_family = AF_INET;
		echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		echoServAddr.sin_port = htons(echoServPort);

		if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) > 0)
				DieWithError("bind() failed");

		if (listen(servSock, MAXPENDING) < 0)
				DieWithError("listen() failed");

		for (; ;) {

				clntLen = sizeof(echoClntAddr);

				if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
						DieWithError("accept() failed");

				printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

				HandleTCPClient(clntSock);
		}
}


void HandleTCPClient(int clntSocket) {
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;
    int a;
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    a = atoi(echoBuffer);
    // a++;
    sum = sum + a;
		// strcpy(echoBuffer, "0");
    sprintf(echoBuffer, "%d ", sum);

    while (recvMsgSize > 0) {

        if (send(clntSocket, echoBuffer, sizeof(echoBuffer), 0) != sizeof(echoBuffer))
            DieWithError("send() failed");

        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");

    }
    close(clntSocket);

		sum =  sum + ch[0];
		if ((ch[1] = sum) >= 30) {
			send(clntSocket, ch, sizeof(ch), 0);
			DieWithError("you are lose");
			close(clntSocket);
			exit(1);
		}

		if (send(clntSocket, ch, sizeof(ch), 0) != sizeof(ch))
			// if (send(clntSocket, echoBuffer, sizeof(echoBuffer), 0) != sizeof(echoBuffer))
			DieWithError("send() failed");

		if ((recvMsgSize = recv(clntSocket, ch, RCVBUFSIZE, 0)) < 0)
			DieWithError("recv() failed");
}
