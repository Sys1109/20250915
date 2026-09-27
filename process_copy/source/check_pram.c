#include <process_copy.h>

int check_pram(int argc, const char * srcfile , int pronum)
{
	if(argc < 3){
		printf("用户参数数量异常\n");
		return -1;
	}	
	if(access(srcfile, F_OK) == -1)
	{
		perror("源文件不存在\n");
		return -1;
	}
	if(pronum <3 || pronum > 100)
	{
		printf("进程数量必须在3~100之间\n");
		return -1;
	}
	return 0;
}
