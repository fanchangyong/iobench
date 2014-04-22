#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>

#include "conn_fork.h"

#define SERVERTYPE_FORK   1
#define SERVERTYPE_THREAD 2

struct config
{
	int isserver;
	unsigned short port;
	char addr[1024];
	int servertype;
};

typedef struct config config;

config conf;

int parse_config(int argc,char** argv)
{
	bzero(&conf,sizeof(conf));
	for(;;)
	{
		int c = getopt(argc,argv,"st");
		switch(c)
		{
			case 's':
				{
					conf.isserver = 1;
					break;
				}
			case 't':
				{
					if(strcmp("fork",optarg)==0)
					{
						conf.servertype = SERVERTYPE_FORK;
					}
					else if(strcmp("thread",optarg)==0)
					{
						conf.servertype = SERVERTYPE_THREAD;
					}
				}
		}
	}

	if(optind<argc)
	{
		strcpy(conf.addr,argv[optind++]);
	}
	
	if(optind<argc)
	{
		conf.port = atoi(argv[optind++]);
	}
	return 0;
}

int main(int argc,char** argv)
{
	parse_config(argc,argv);
	if(conf.isserver)
	{
		if(conf.servertype==SERVERTYPE_FORK)
		{
			do_conn_fork(conf.port);
		}
	}
	else
	{

	}
}

