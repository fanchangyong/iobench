#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "../deps/sockutil/sock_util.h"

int reg_kqueue_r(int kq,int fd,int udata)
{
	int *pudata = malloc(sizeof(int));
	*pudata = udata;
	struct kevent events[1];
	EV_SET(&events[0],fd,EVFILT_READ,EV_ADD,0,9,pudata);
	if(kevent(kq,events,1,NULL,0,NULL)==-1)
	{
		return -1;
	}
	return 0;
}

int del_kqueue(int kq,int fd)
{
	struct kevent events[1];
	EV_SET(&events[0],fd,EVFILT_READ,EV_DELETE,0,0,NULL);
	if(kevent(kq,events,1,NULL,0,NULL)==-1)
	{
		return -1;
	}
	return 0;
}

int wait_event(int kq)
{
	const int len = 1024;
	struct kevent events[len];
	int n = kevent(kq,NULL,0,events,len,NULL);
	if(n==-1)
	{
		return -1;
	}

	int i;
	for(i=0;i<n;i++)
	{
		struct kevent *pe = &events[i];
		int fd=pe->ident;
		int udata = *(int*)(pe->udata);
		if(pe->flags & EV_EOF)
		{
			del_kqueue(kq,fd);
		}
		else if(udata==1)
		{
			struct sockaddr_in addr;
			socklen_t len = sizeof(addr);
			int cli_fd;
			if((cli_fd=accept(fd,(struct sockaddr*)&addr,&len))==-1)
			{
				perror("accept");
				return -1;
			}
			printf("client fd:%d\n",cli_fd);
			if(reg_kqueue_r(kq,cli_fd,2)==-1)
			{
				perror("reg kqueue");
				return -1;
			}
		}
		else if(udata==2)
		{
			int sz = pe->data;
			char *buf = malloc(sz);
			read(fd,buf,sz);
			write(fd,buf,sz);
		}
	}
	return 0;
}

int do_conn_kqueue(unsigned short port)
{
	int kq = kqueue();
	if(kq==-1)
	{
		perror("kqueue");
		return -1;
	}

	int fd = tcp_server(port);
	if(fd==-1)
	{
		perror("tcp_server");
		return 1;
	}

	if(reg_kqueue_r(kq,fd,1)==-1)
	{
		perror("kevent");
		return -1;
	}

	printf("Server Startupt at : %d\n",port);

	for(;;)
	{
		wait_event(kq);
	}
	return 0;
}

