#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main(void){
	pid_t pid1,pid2,pid3;
	printf("父进程PID=%d\n",getpid());
	pid1=fork();
	if(pid1==0){
		printf("子进程PID=%d,准备setsid脱离终端\n",getpid());
		setsid();
		while(1){
			sleep(2);
			printf("【子1】我还活着！pid=%d\n",getpid());
		}
		return 0;
	}
	pid2=fork();
	if(pid2==0){
		printf("子进程2PID=%d,没有脱离终端\n",getpid());
		while(1){
			sleep(2);
			printf("【子2】我还活着！pid=%d\n",getpid());
		}
		return 0;
	}
	pid3=fork();
	if(pid3==0){
		printf("子进程3PID=%d,没有脱离终端\n",getpid());
		while(1){
			sleep(2);
			printf("【子3】我还活着！pid=%d\n",getpid());
			}
			return 0;
	}
	wait(NULL);
   return 0;
}
