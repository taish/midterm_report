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

#define RCVBUFSIZE 32

int main(int argc, char *argv[]) {
		int sock;
		struct sockaddr_in echoServAddr;
		unsigned short echoServPort;
		char *servIP;
		char *echoString;
		char echoBuffer[RCVBUFSIZE];
		unsigned int echoStringLen;
		int byteRcvd, totalBytesRcvd;
		int ch[2];
		char *echoFirst;

		if ((argc < 3) || (argc > 4 )) {
			fprintf(stderr, "Usage: %s <Server IP>  [<Echo Port>]\n", argv[0]);
			exit(1);
		}

		servIP = argv[1];
		echoString = argv[2];

		if (argc == 4)
				echoServPort = atoi(argv[3]);
		else
				echoServPort = 7;

		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
				DieWithError("socket() failed");

		memset(&echoServAddr, 0, sizeof(echoServAddr));
		echoServAddr.sin_family = AF_INET;
		echoServAddr.sin_addr.s_addr = inet_addr(servIP);
		echoServAddr.sin_port = htons(echoServPort);

		if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
			DieWithError("connect() failed");

		printf("geme start\n");

		echoFirst = "hello";
		send(sock, echoFirst, sizeof(echoFirst), 0);
    recv(sock, echoBuffer, RCVBUFSIZE, 0);
		printf("%s\n", echoBuffer);


		for (;;) {
			scanf("%d",&ch[0]);

			if (send(sock, ch, sizeof(ch), 0) != sizeof(ch))
					DieWithError("send() failed");
			totalBytesRcvd = 0;
			printf("Received: ");
			while (totalBytesRcvd < sizeof(ch)) {
					if ((byteRcvd = recv(sock, ch, RCVBUFSIZE - 1, 0)) <= 0)
							DieWithError("recv() failed or connection closed prematurely");
					totalBytesRcvd += byteRcvd;
					printf("%d", ch[1]);
					printf("\n");
			}
		}

		close(sock);

		exit(0);
}
