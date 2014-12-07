//
//  main.c
//  tcp_socket
//
//  Created by 小林大志 on 2014/11/06.
//  Copyright (c) 2014年 小林大志. All rights reserved.
//
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "die_with_error.h"

#define RCVBUFSIZE 1000

void HandleTCPClient(int clntSocket) {
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;
    int a;
    int static sum = 0;

    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    a = atoi(echoBuffer);
    // a++;
    sum = sum + a;
    sprintf(echoBuffer, "%d", sum);

    while (recvMsgSize > 0) {
        // printf(echoBuffer);
        // printf(a);
        // echoBuffer = a;
        // send(clntSocket, 'child process\n', recvMsgSize, 0);

        if (send(clntSocket, echoBuffer, sizeof(echoBuffer), 0) != sizeof(echoBuffer))
            DieWithError("send() failed");
        printf(echoBuffer);

        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");

    }
    close(clntSocket);
}
