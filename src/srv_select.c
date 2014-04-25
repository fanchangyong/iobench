#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <errno.h>
#include "../deps/sockutil/sock_util.h"

extern int errno;
static fd_set readfds;
static fd_set writefds;
static fd_set excpfds;

int do_srv_select(unsigned short port)
{
	int fd = tcp_server(port);
	if(fd==-1)
	{
		perror("tcp_server");
		return 1;
	}

	int on = 1;
	if(-1==ioctl(fd,FIONBIO,(char*)&on))
	{
		perror("ioctl");
		return -1;
	}

	printf("Server Startupt at : %d\n",port);
	signal(SIGCHLD,SIG_IGN);

	FD_ZERO(&readfds);
	FD_SET(fd,&readfds);

	int curmax = fd;

	fd_set readfds_backup;
	memcpy(&readfds_backup,&readfds,sizeof(fd_set));

	for(;;)
	{
		memcpy(&readfds,&readfds_backup,sizeof(fd_set));
		int n = select(curmax+1,&readfds,NULL,NULL,NULL);
		printf("n:%d\n",n);
		if(FD_ISSET(fd,&readfds))
		{
			for(;;)
			{
				struct sockaddr_in cli_addr;
				int cli_sock = tcp_accept(fd,&cli_addr);
				if(cli_sock==-1)
				{
					if(errno!=EAGAIN)
						perror("tcp_accept");
					break;
				}
				if(cli_sock>curmax)
				{
					curmax=cli_sock;
				}
				printf("cli sock:%d\n",cli_sock);
				FD_SET(cli_sock,&readfds_backup);
			}
		}
		else
		{
			int i;
			for(i = 0;i <= curmax+1;i++)
			{
				int close_conn = 0;
				if(FD_ISSET(i,&readfds))
				{
					static char buf[1024];
					int n=read(i,buf,sizeof(buf));
					if(n<0)
					{
						if(errno!=EAGAIN)
						{
							perror("read");
							close_conn = 1;
						}
					}
					else if(n==0)
					{
						printf("*Connection Closed\n");
						close_conn = 1;
					}
					else if(n>0)
					{
						printf("buf:%s\n",buf);
					}

					if(close_conn)
					{
						close(i);
						FD_CLR(i,&readfds_backup);
						if(i==curmax)
						{
							while(FD_ISSET(curmax,&readfds_backup)==0)
							{
								--curmax;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

