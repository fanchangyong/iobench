#ifndef __IOBENCH_CONFIG__
#define __IOBENCH_CONFIG__

struct config
{
	int isserver;
	unsigned short port;
	char addr[1024];
	int servertype;
	int conncount;
	int writelen; // write buf length
	int writecount; // write count
};

typedef struct config config;

int parse_config(config *conf,int argc,char** argv);

#define SERVERTYPE_FORK   1
#define SERVERTYPE_THREAD 2
#define SERVERTYPE_KQUEUE 3
#define SERVERTYPE_SELECT 4

#endif
