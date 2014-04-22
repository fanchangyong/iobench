#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../deps/sockutil/sock_util.h"

void process_conn(int fd)
{
	char buf[]="hello,client!";
	for(;;)
	{
		char * rbuf = malloc(1024);
		int rn = read(fd,rbuf,1024);
		if(rn<=0)
		{
			return;
		}
		else
		{
			printf("client said:%s\n",rbuf);
		}
		int n = write(fd,buf,sizeof(buf)+1);
		if(n==-1)
		{
			perror("write");
			return;
		}
	}
}

int main(int argc,char** argv)
{
	int fd = tcp_server(8888);
	if(fd==-1)
	{
		perror("tcp_server");
		return 1;
	}

	for(;;)
	{
		struct sockaddr_in cli_addr;
		int cli_sock = tcp_accept(fd,&cli_addr);
		if(cli_sock==-1)
		{
			perror("tcp_accept");
			return 1;
		}

		// create process to handle connection
		pid_t pid = fork();
		if(pid<0)
		{
			perror("fork");
			return 1;
		}
		else if(pid==0)
		{
			// child process
			process_conn(cli_sock);
		}
	}
	return 0;
}

