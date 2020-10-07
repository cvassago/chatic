#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int client(uint16_t port, char* src, char* dst)
{
	int sd;
	struct sockaddr_in addr;
	
	if ((-1) == (sd = socket(AF_INET,SOCK_STREAM, 0)))
	{
		perror("socket");
		return __LINE__;
	}
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	sleep(3);

	if (connect(sd, (const struct sockaddr*)&addr,
				 sizeof(struct sockaddr_in)))
	{
		perror("connect");
		close(sd);
		return __LINE__;
	}

	write(sd, "Hello, World\n", 13);
	close(sd);
	return 0;
}

int server(uint16_t port, char* src, char* dst)
{
	int sd, clsd;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char msg[13]="ERROR!!!!!!!\n";

	if ((-1) == (sd = socket(AF_INET,SOCK_STREAM, 0)))
	{
		perror("socket");
		return __LINE__;
	}
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(sd, (const struct sockaddr*)&addr,
				 sizeof(struct sockaddr_in)))
	{
		perror("bind");
		close(sd);
		return __LINE__;
	}

	if ((-1) == listen(sd, 1))
	{
		perror("listen");
		close(sd);
		return __LINE__;
	}
	addr_size = sizeof(struct sockaddr_in);
	if ((-1) == (clsd = accept(sd, (struct sockaddr*) &addr, &addr_size)))
	{
		perror("listen");
		close(sd);
		return __LINE__;
	}
	read(clsd, msg, 13);
	write(1,msg,13);
	close(clsd);
	close(sd);
	return 0;
}

int main(int argc, char* argv[])
{
	pid_t pid;
	uint16_t port;

	if (argc != 3)
	{
		fprintf(stderr,"Usage: %s <src> <dst>\n",argv[0]);
		return 0;
	}
	port = getpid();
	if (port <= 1000 ) port += 1000;
	if ((-1) == (pid = fork()))
	{
		perror("fork");
		return __LINE__;
	}
	return ((pid != 0)?(server):(client))(port,argv[1],argv[2]);
}
