#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(int arge, char** argv){
	int sfd;
	int dfd;
	int offset = atoi(argv[3]);
	int blocksize = atoi(argv[4]);
	if((sfd = open(argv[1],0,O_RDONLY))==-1)
		perror("open srcfile , failed");
	if((dfd = open(argv[2],O_WRONLY|O_CREAT,0664)) == -1)
		perror("open srcfile , failed");
	lseek(sfd,offset,SEEK_SET);
	lseek(dfd,offset,SEEK_SET);

	char buffer[blocksize];
	int len;
	printf("child p %d execl Copy , offset %d , blocksize %d\n",getpid(),offset,blocksize);
	len = read(sfd,buffer,sizeof(buffer));
	write(dfd,buffer,len);
	close(sfd);
	close(dfd);
	return 0;
}
