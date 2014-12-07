
all:
	gcc TCPEchoServer-Fork.c die_with_error.c AcceptTCPConnection.c CreateTCPServerSocket.c -o gameserver

c:
	gcc tcp_client.c die_with_error.c -o client
clean:
	rm server
