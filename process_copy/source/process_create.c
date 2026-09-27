#include <process_copy.h>

int process_create(const char *srcfile, const char *destfile,int pronum ,int blocksize)
{
	pid_t pid;
	char offset_str[32];
	char block_str[32];
	for(int i=0;i<pronum;i++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork fail");
			return -1;
		}
		if(pid == 0)
		{
			off_t offset = (off_t)i * blocksize;
			sprintf(offset_str,"%ld",offset);
			sprintf(block_str,"%d",blocksize);
			execl("/home/colin/20250915/process_copy/MOD/Copy","Copy",srcfile,destfile,offset_str,block_str,NULL);
			perror("execl fail");
			exit(1);
		}
	}
	return 0;
}
