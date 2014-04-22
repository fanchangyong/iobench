#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sock_util.h"
#include <pthread.h>

/*
 * Usage:
 * -s server
 * -c client
 * -h host
 * -p port
 * -m max connection
 * -t thread count
 * -b backlog
 */

int max_conn = 0;
int server_flag = 0;
int client_flag = 0;
unsigned short port = 8888;
const char* host = "127.0.0.1";
int thread_count = 0;
int backlog = 5;

// statistics
int conn_count = 0;

int parse_flag(int argc,char** argv)
{
	int c;
	while( (c=getopt(argc,argv,"msp:h:"))!=-1 )
	{
		switch(c)
		{
			case 'm':
				max_conn = 1;
				break;
			case 's':
				server_flag = 1;
				break;
			case 'c':
				client_flag = 1;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'h':
				host = optarg;
				break;
			default:
				printf("** unknown option:%c",c);
				break;
		}
	}
	return 0;
}

void do_srv()
{
	int fd = tcp_server(8888);
	if(fd==-1)
	{
		perror("tcp_server");
		return;
	}

	for(;;)
	{
		struct sockaddr_in cli_addr;
		int cli_fd = tcp_accept(fd,&cli_addr);
		printf("fd:%d\n",fd);
		if(cli_fd==-1)
		{
			perror("accept");
			return;
		}
		conn_count++;
	}
}

void* statistics(void* arg)
{
	for(;;)
	{
		printf("connection count:%d\n",conn_count);
		sleep(1);
	}
}

int main(int argc,char **argv)
{
	parse_flag(argc,argv);
	if(server_flag)
	{
		pthread_t tid;
		pthread_create(&tid,NULL,statistics,NULL);
		do_srv();
	}
	return 0;
}
