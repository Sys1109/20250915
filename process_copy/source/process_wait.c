#include <process_copy.h>

void process_wait(void)
{
	pid_t wpid;
	int status;
	while((wpid = wait(&status)) > 0)
	{
		printf("child process %d\n",wpid);	
	}
}
