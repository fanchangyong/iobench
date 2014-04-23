#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>

#include "conn_fork.h"
#include "cli.h"
#include "config.h"

extern int errno;

config conf;

int main(int argc,char** argv)
{
	bzero(&conf,sizeof(config));
	parse_config(&conf,argc,argv);
	if(conf.isserver)
	{
		if(conf.servertype==SERVERTYPE_FORK)
		{
			do_conn_fork(conf.port);
		}
	}
	else
	{
		static char buf[]="HELLO";
		if(do_conn(conf.addr,conf.port,conf.conncount,buf,sizeof(buf))==-1)
		{
			printf("conn error:%s\n",strerror(errno));
		}
	}
}

