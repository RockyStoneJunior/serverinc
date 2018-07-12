#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

struct arg{
	int serverfd;
	int clientfd;
};

void *connetion_process(void *);

int main(int argc, char **argv)
{
	char recv_buff[1024];
	char *response = "Hello Stone!\r\n";
	int response_len = strlen(response);

	struct sockaddr_in servaddr;

	struct arg *connection_thread_arg;
	
	pthread_t connection_process_thread;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8888);
	
	int servfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(servfd, 5);

	int servaddr_len = sizeof(servaddr);

	while(1)
	{
		int clientfd = accept(servfd, (struct sockaddr *)&servaddr, &servaddr_len);

		connection_thread_arg = malloc(sizeof(struct arg));
		connection_thread_arg->serverfd = servfd;
		connection_thread_arg->clientfd = clientfd;
		pthread_create(&connection_process_thread, NULL, connetion_process, connection_thread_arg);
	}
	
	return 0;
}

void *connetion_process(void *arg)
{	
	char *recv_buff[1024];
	int recv_len;
	
	pid_t pid = syscall(SYS_gettid);
	
	int serverfd = ((struct arg*)arg)->serverfd;
	int clientfd = ((struct arg*)arg)->clientfd;

	while(1)
	{
		while((recv_len = read(clientfd, recv_buff, 1024)) > 0)
		{
			write(STDOUT_FILENO, recv_buff, recv_len);
		}
		
		printf("%d: No message now!\r\n",pid);
	}

	return NULL;
}
