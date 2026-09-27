#include <process_copy.h>

int block_cur(const char *srcfile,int pronum)
{
	struct stat st;
	if(stat(srcfile, &st) == -1)
	{
		perror("stat fail");
		return -1;
	}
	off_t filesize = st.st_size;
	if(filesize % pronum == 0)
	{
		return filesize / pronum;	
	}
	else
	{
		return filesize / pronum + 1;	
	}
}
