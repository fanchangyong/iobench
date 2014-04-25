#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "cli.h"
#include "../deps/sockutil/sock_util.h"

int do_conn(const char* addr,unsigned short port,int count,int writelen,int writecount)
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
		char* buf = malloc(writelen);
		if(buf && writelen>0)
		{
			int j;
			for(j = 0;j < writecount;j++)
			{
				if(write(fd,buf,writelen)==-1)
				{
					perror("write");
					return -1;
				}
			}
		}
		free(buf);
		time_t t2 = time(NULL);
		sumtime += (t2 - t1);
	}
	printf("average response time:%ld\n",sumtime/count);
	return 0;
}
