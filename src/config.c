#include <strings.h>
#include <getopt.h>
#include "config.h"
#include <stdlib.h>
#include <string.h>

int ensure_default(config *conf);

int parse_config(config *conf,int argc,char** argv)
{
	bzero(conf,sizeof(config));
	for(;;)
	{
		int c = getopt(argc,argv,"st:p:a:c:w:W:f:");
		if(c==-1)
			break;
		switch(c)
		{
			case 's':
				{
					conf->isserver = 1;
					break;
				}
			case 'f':
				{
					conf->isfile = 1;
					strncpy(conf->path,optarg,sizeof(conf->path));
				}
			case 't':
				{
					if(strcmp("fork",optarg)==0)
					{
						conf->servertype = SERVERTYPE_FORK;
					}
					else if(strcmp("thread",optarg)==0)
					{
						conf->servertype = SERVERTYPE_THREAD;
					}
					else if(strcmp("kqueue",optarg)==0)
					{
						conf->servertype = SERVERTYPE_KQUEUE;
					}
					else if(strcmp("select",optarg)==0)
					{
						conf->servertype = SERVERTYPE_SELECT;
					}
					else if(strcmp("poll",optarg)==0)
					{
						conf->servertype = SERVERTYPE_POLL;
					}
					else if(strcmp("epoll",optarg)==0)
					{
						conf->servertype = SERVERTYPE_EPOLL;
					}
					break;
				}
			case 'p':
				{
					conf->port = atoi(optarg);
					break;
				}
			case 'a':
				{
					strncpy(conf->addr,optarg,sizeof(conf->addr));
					break;
				}
			case 'c':
				{
					conf->conncount = atoi(optarg);
					break;
				}
			case 'w':
				{
					conf->writelen = atoi(optarg);
					break;
				}
			case 'W':
				{
					conf->writecount = atoi(optarg);
					break;
				}
		}
	}
	ensure_default(conf);

	return 0;
}

int ensure_default(config *conf)
{
	if(conf->servertype==0)
	{
		conf->servertype=SERVERTYPE_KQUEUE;
	}
	if(conf->port==0)
	{
		conf->port=8888;
	}
	if(strcmp(conf->addr,"")==0)
	{
		strncpy(conf->addr,"127.0.0.1",sizeof(conf->addr));
	}
	if(conf->conncount == 0)
	{
		conf->conncount = 1;
	}
	if(conf->writelen==0)
	{
		conf->writelen = 1024;
	}
	if(conf->writecount==0)
	{
		conf->writecount = 1;
	}
	return 0;
}

