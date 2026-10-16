#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int i;
	for (i = 0;i < 3;i++) {
		pid = fork();
		if (pid < 0) {
			perror("fork call failed");
			exit(0);
		}
		else if (pid == 0) {
			setsid();
			printf("子进程%d pid = %d\n ", i + 1, getpid());
			while (1)
			{
				sleep(1);
			}
			exit(1);
		}
	}
	printf("父进程 pid = %d", getpid());
	while (1) {
		sleep(1);
	}

	return 0;
}
