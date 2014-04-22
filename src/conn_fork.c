#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "../deps/sockutil/sock_util.h"

int process_conn(int fd)
{
	char buf[]="hello,client!";
	for(;;)
	{
		char * rbuf = malloc(1024);
		int rn = read(fd,rbuf,1024);
		if(rn==0)
		{
			printf("client over");
			return 0;
		}
		else if(rn<0)
		{
			perror("read");
			return 1;
		}
		else
		{
			printf("client said:%s\n",rbuf);
		}
		int n = write(fd,buf,sizeof(buf)+1);
		if(n==-1)
		{
			perror("write");
			return 1;
		}
	}
	return 0;
}

int do_conn_fork(unsigned short port)
{
	int fd = tcp_server(port);
	if(fd==-1)
	{
		perror("tcp_server");
		return 1;
	}

	signal(SIGCHLD,SIG_IGN);

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
			exit(process_conn(cli_sock));
		}
	}
	return 0;
}

