#include <process_copy.h>

int block_cur(const char *srcfile,int pronum)
{
	int fsize;
	int fd = open(srcfile,O_RDONLY);
	fsize = lseek(fd,0,SEEK_END);
	
	if(fsize % pronum==0)
		return fsize / pronum;
	else
		return fsize / pronum +1;
}
