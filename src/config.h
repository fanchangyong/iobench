#ifndef __IOBENCH_CONFIG__
#define __IOBENCH_CONFIG__

struct config
{
	int isserver;
	int isfile;
	unsigned short port;
	char addr[1024];
	int servertype;
	int conncount;
	int writelen; // write buf length
	int writecount; // write count
	char path[1024];
};

typedef struct config config;

int parse_config(config *conf,int argc,char** argv);

#define SERVERTYPE_FORK   1
#define SERVERTYPE_THREAD 2
#define SERVERTYPE_KQUEUE 3
#define SERVERTYPE_SELECT 4
#define SERVERTYPE_POLL   5

#endif
