#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <math.h>

#include "srv_epoll.h"
#include "srv_poll.h"
#include "fileio.h"
#include "srv_select.h"
#include "conn_kqueue.h"
#include "conn_fork.h"
#include "conn_thread.h"
#include "cli.h"
#include "config.h"

extern int errno;

config conf;

int main(int argc,char** argv)
{
	/*long n  = sysconf(_SC_OPEN_MAX);*/
	/*printf("n:%ld\n",n);*/
	/*return -1;*/


	/*struct rlimit rl;*/
	/*if(getrlimit(RLIMIT_NOFILE,&rl)==-1)*/
	/*{*/
		/*perror("getrlimit");*/
		/*return -1;*/
	/*}*/

	/*if(rl.rlim_max<OPEN_MAX)*/
	/*{*/
		/*rl.rlim_cur = rl.rlim_max;*/
	/*}*/
	/*else*/
	/*{*/
		/*rl.rlim_cur = OPEN_MAX;*/
	/*}*/
	
	/*if(setrlimit(RLIMIT_NOFILE,&rl)==-1)*/
	/*{*/
		/*printf("errno:%d\n",errno);*/
		/*perror("setrlimit");*/
		/*return -1;*/
	/*}*/

	/*// print rlimit*/
	/*if(getrlimit(RLIMIT_NOFILE,&rl)==-1)*/
	/*{*/
		/*perror("getrlimit");*/
		/*return -1;*/
	/*}*/
	/*printf("now limit:%lld,%lld\n",rl.rlim_cur,rl.rlim_max);*/


	bzero(&conf,sizeof(config));
	parse_config(&conf,argc,argv);
	if(conf.isfile)
	{
		write_file(conf.path,conf.writelen,conf.writecount);
	}
	else if(conf.isserver)
	{
		if(conf.servertype==SERVERTYPE_FORK)
		{
			do_conn_fork(conf.port);
		}
		else if(conf.servertype==SERVERTYPE_THREAD)
		{
			do_conn_thread(conf.port);
		}
		else if(conf.servertype==SERVERTYPE_KQUEUE)
		{
			do_conn_kqueue(conf.port);
		}
		else if(conf.servertype==SERVERTYPE_SELECT)
		{
			do_srv_select(conf.port);
		}
		else if(conf.servertype==SERVERTYPE_POLL)
		{
			do_srv_poll(conf.port);
		}
		else if(conf.servertype==SERVERTYPE_EPOLL)
		{
			do_srv_epoll(conf.port);
		}
	}
	else
	{
		if(do_conn(conf.addr,conf.port,conf.conncount,conf.writelen,conf.writecount)==-1)
		{
			printf("conn error:%s\n",strerror(errno));
		}
	}
}

