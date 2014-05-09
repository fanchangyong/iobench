#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include "../deps/sockutil/sock_util.h"



int reg_epoll_r(int epfd,int fd,int udata)
{
	struct epoll_event ev;
	bzero(&ev,sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	//ev.data.u32 = udata;
	return epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
}

int del_epoll(int epfd,int fd)
{
	epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
}

static int wait_event(int epfd,int listenfd)
{
	const int len = 1024;
	struct epoll_event events[len];
	int n=epoll_wait(epfd,events,len,-1);
	if(n==-1)
	{
		return -1;
	}

	printf("n:%d\n",n);

	int i;
	for(i=0;i<n;i++)
	{
		struct epoll_event *pe = &events[i];
		int fd=pe->data.fd;
		int udata = pe->data.u32;
		if(pe->events & EPOLLRDHUP)
		{
			printf("event read hup\n");
			close(fd);
			del_epoll(epfd,fd);
		}
		else if(pe->events&EPOLLIN && pe->data.fd==listenfd)
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
			if(reg_epoll_r(epfd,cli_fd,22)==-1)
			{
				perror("reg epoll");
				return -1;
			}
		}
		else
		{
			char buf[1024];
			int rn=read(fd,buf,sizeof(buf));
			if(rn==0)
			{
				printf("read eof\n");
				close(fd);
				del_epoll(epfd,fd);
			}
			else if(rn<0)
			{
				perror("read");
			}
			write(fd,buf,rn);
		}
	}
	return 0;
}

int do_srv_epoll(unsigned short port)
{
	int epfd = epoll_create(1);
	printf("epfd:%d\n",epfd);
	if(epfd==-1)
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

	if(reg_epoll_r(epfd,fd,11)==-1)
	{
		perror("kevent");
		return -1;
	}

	printf("Server Startupt at : %d\n",port);

	for(;;)
	{
		if(wait_event(epfd,fd)==-1)
			return -1;
	}
	return 0;
}

