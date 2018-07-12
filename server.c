#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	char recv_buff[1024];
	char *response = "Hello Stone!\r\n";
	int response_len = strlen(response);

	struct sockaddr_in servaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8888);
	
	int servfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(servfd, 5);

	int servaddr_len = sizeof(servaddr);
	int clientfd = accept(servfd, (struct sockaddr *)&servaddr, &servaddr_len);

	int recv_len = read(clientfd, recv_buff, 1024);
	write(clientfd, response, response_len);
	write(STDOUT_FILENO, recv_buff, recv_len);
	
	return 0;
}
