#include <strings.h>
#include <getopt.h>
#include "config.h"
#include <stdlib.h>

int parse_config(config *conf,int argc,char** argv)
{
	for(;;)
	{
		int c = getopt(argc,argv,"st:p:a:c:");
		if(c==-1)
			break;
		switch(c)
		{
			case 's':
				{
					conf->isserver = 1;
					break;
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
		}
	}

	return 0;
}

