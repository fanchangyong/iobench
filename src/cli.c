#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "cli.h"
#include "../deps/sockutil/sock_util.h"

int do_conn(const char* addr,unsigned short port,int count,const char* buf,int buflen)
{
	time_t sumtime = 0;
	int i;
	for(i=0;i<count;i++)
	{
		time_t t1 = time(NULL);
		int fd = tcp_conn(addr,port);
		if(fd==-1)
		{
			return -1;
		}
		if(buf && buflen>0)
		{
			if(write(fd,buf,buflen)==-1)
			{
				perror("write");
				return -1;
			}
		}
		time_t t2 = time(NULL);
		sumtime += (t2 - t1);
	}
	printf("average response time:%ld\n",sumtime/count);
	return 0;
}
