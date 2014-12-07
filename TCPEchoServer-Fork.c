#include "TCPEchoServer.h"
#include "sys/wait.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "die_with_error.h"

#define RCVBUFSIZE 32

static int sum = 0;
static int first = 1;

void HandleTCPClient(int clntSocket);
int youAreFirst(int yes);

int main(int argc, char *argv[]) {
	int servSock;
	// int clntSock;
	int clntParentSock;
	int clntChildSock;

	unsigned short echoServPort;
	pid_t processID;
	unsigned int childProcCount = 0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <Server Port>", argv[0]);
		exit(1);
	}

	echoServPort = atoi(argv[1]);

	servSock = CreateTCPServerSocket(echoServPort);

	for(;;) {

		if ((processID = fork()) < 0)
			DieWithError("fork() failed");
		else if (processID == 0) {
			for (; ;) {
				clntChildSock = AcceptTCPConnection(servSock);
				close(servSock);
	      youAreFirst(clntChildSock);
				HandleTCPClient(clntChildSock);
				exit(0);
			}
		} else {
			for (;;) {
				clntParentSock = AcceptTCPConnection(servSock);
				youAreFirst(clntParentSock);
				HandleTCPClient(clntParentSock);
				exit(0);
			}
		}

		printf("with child process: %d\n", (int) processID);
		close(clntParentSock);
		childProcCount++;

		while (childProcCount) {
			processID = waitpid((pid_t) -1, NULL, WNOHANG);
			if (processID < 0)
				DieWithError("waitpid() failed");
			else if (processID == 0) {
				break;
			} else {
				childProcCount--;
			}
		}
	}
}

void HandleTCPClient(int clntSocket) {
	int *echoBuffer;
  int recvMsgSize;
	int ch[2];

  if ((recvMsgSize = recv(clntSocket, ch, RCVBUFSIZE, 0)) < 0)
    DieWithError("recv() failed");
		// send(clntSocket, ch, sizeof(ch), 0);
  sum =  sum + ch[0];

  while (recvMsgSize > 0) {


		if ((ch[1] = sum) >= 30) {
			send(clntSocket, ch, sizeof(ch), 0);
      DieWithError("you are lose");
	    close(clntSocket);
			exit(1);
		}

    if (send(clntSocket, ch, sizeof(ch), 0) != sizeof(ch))
      DieWithError("send() failed");
    }
    if ((recvMsgSize = recv(clntSocket, ch, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
    close(clntSocket);
}

int youAreFirst(int clntSocket) {
  char *echoBuffer;
  int recvMsgSize;
	if (first == 1) {
		first = 0;
    if ((recvMsgSize = recv(clntSocket, &echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
		// printf("%s\n", echoBuffer);
		echoBuffer = "connection good. you are first turn";
	  if (send(clntSocket, &echoBuffer, sizeof(echoBuffer), 0) != sizeof(echoBuffer))
	      DieWithError("send() failed");
		return 1;
	} else {
    if ((recvMsgSize = recv(clntSocket, &echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
		// printf("%s\n", echoBuffer);
		echoBuffer = "connection good. you are second turn";
	  if (send(clntSocket, &echoBuffer, sizeof(echoBuffer), 0) != sizeof(echoBuffer))
	      DieWithError("send() failed");
		return 0;
	}
}
