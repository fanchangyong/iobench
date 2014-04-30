#include "fileio.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

static long sumwritten = 0;

void* statics(void* p)
{
	int lastwritten = 0;
	for(;;)
	{
		printf("write speed: %ldMB/s\n",sumwritten-lastwritten);
		lastwritten=sumwritten;
		sleep(1);
	}
	return 0;
}

int write_file(const char* path,int bufsize,int writecount)
{
	printf("bufsize:%d,writecount:%d\n",bufsize,writecount);
	unlink(path);
	int fd = open(path,O_RDWR|O_CREAT,0744);
	if(fd==-1)
	{
		printf("path:%s\n",path);
		perror("open");
		return -1;
	}

	int writesize = bufsize*1024*1024; // trans to MB

	char* buf=malloc(writesize);
	if(buf==NULL)
	{
		perror("malloc");
		return -1;
	}
	pthread_t threadid;
	/*if(pthread_create(&threadid,NULL,statics,NULL)!=0)*/
	/*{*/
		/*perror("pthread_create");*/
		/*return -1;*/
	/*}*/
	time_t t1 = time(NULL);

	int i;
	for(i=0;i<writecount;i++)
	{
		int written = write(fd,buf,writesize);
		if(written==-1)
		{
			perror("write");
			return -1;
		}
		sumwritten += written;
	}
	time_t t2 = time(NULL);
	printf("has written:%ld MBs in %ld seconds\n",sumwritten/1024/1024,t2-t1);
	return 0;
}
