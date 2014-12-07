#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void DieWithError(char *errorMessage);

int AcceptTCPConnection(int servSock) {
	int clntSock;
	struct sockaddr_in echoClntAddr;
	unsigned int clntLen;

	clntLen = sizeof(echoClntAddr);

	if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
		DieWithError("socket() failed");

	printf("Handing client %s\n", inet_ntoa(echoClntAddr.sin_addr));

	return clntSock;
}
