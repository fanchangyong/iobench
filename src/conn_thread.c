#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "../deps/sockutil/sock_util.h"

static void* process_conn(void* pfd)
{
	int fd = *(int*)pfd;
	char buf[]="hello,client!";
	for(;;)
	{
		char * rbuf = malloc(1024);
		int rn = read(fd,rbuf,1024);
		if(rn==0)
		{
			printf("**client over**\n");
			return 0;
		}
		else if(rn<0)
		{
			perror("read");
			return NULL;
		}
		else
		{
			printf("client said:%s\n",rbuf);
		}
		int n = write(fd,buf,sizeof(buf)+1);
		if(n==-1)
		{
			perror("write");
			return NULL;
		}
	}
	free(pfd);
	return 0;
}

int do_conn_thread(unsigned short port)
{
	int fd = tcp_server(port);
	if(fd==-1)
	{
		perror("tcp_server");
		return 1;
	}

	printf("Server Startupt at : %d\n",port);
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
		pthread_t threadid;
		int *pfd = malloc(sizeof(int));
		*pfd = cli_sock;
		int ret = pthread_create(&threadid,NULL,process_conn,pfd);
		if(ret!=0)
		{
			perror("pthread_create");
		}
	}
	return 0;
}

