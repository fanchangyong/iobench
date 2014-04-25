
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#include "../deps/sockutil/sock_util.h"

static struct pollfd pollfds[1024];
static int index = 0;

int reg_pollfd_r(int fd)
{
	pollfds[index].fd = fd;
	pollfds[index].events = POLLIN;
	pollfds[index].revents = 0;
	++index;
	return 0;
}

int do_srv_poll(unsigned short port)
{
	int fd = tcp_server(port);
	if(fd==-1)
	{
		perror("tcp_server");
		return 1;
	}

	printf("Server Startupt at : %d\n",port);
	signal(SIGCHLD,SIG_IGN);

	reg_pollfd_r(fd);

	for(;;)
	{
		int n = poll(pollfds,index,-1);
		printf("poll:%d\n",n);
		int i;
		for(i = 0;i < index;i++)
		{
			if(pollfds[i].revents & POLLIN)
			{
				if(pollfds[i].fd == fd)
				{
					struct sockaddr_in cli_addr;
					int cli_sock = tcp_accept(fd,&cli_addr);
					if(cli_sock==-1)
					{
						perror("tcp_accept");
						break;
					}
					printf("cli sock:%d\n",cli_sock);
					reg_pollfd_r(cli_sock);
				}
			}
			else
			{
				char buf[1024];
				int readn = read(pollfds[i].fd,buf,sizeof(buf));
				if(readn>0)
				{
					printf("read:%s\n",buf);
				}
			}
		}
	}
	return 0;
}

