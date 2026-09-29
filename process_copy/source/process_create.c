#include <process_copy.h>

int process_create(const char *srcfile, const char *destfile,int pronum ,int blocksize)
{
	char offset_str[50];
	char blocksize_str[50];

	pid_t pid;
	int i;
	for(i=0;i<pronum;i++)
	{
		pid = fork();
		if(pid == 0)
			break;
	}
	if(pid > 0){
		process_wait();
	}else if(pid == 0){
		int offset;
		offset = i * blocksize;
		sprintf(offset_str,"%d",offset);
		sprintf(blocksize_str,"%d",blocksize);
		execl("/home/colin/20250915/process_copy/MOD/Copy","Copy",srcfile,destfile,offset_str,blocksize_str,NULL);
	}else{
		perror("fork call failed");
	}
	return 0;
}
