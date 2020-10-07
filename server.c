#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	int					sd;
	int					cd;
	int					ret;
	char				buf[16];
	struct sockaddr_in	addr;

	if (0 > (sd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket");
		return (__LINE__);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sd, (struct sockaddr*) & addr, sizeof(addr));
	listen(sd, 5);
	cd = accept(sd, NULL, NULL);
	ret = read(cd, buf, 16);
	write(1, buf, ret);
	close(cd);
	close(sd);
	return (0);
}
