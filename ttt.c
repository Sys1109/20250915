#include<stdio.h>
#include<sys/stat.h>
#include<regex.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/fcntl.h>
#include<unistd.h>

int main(void){
    char * regstr = "<a[^>]\\+\\?href=\"\\([^\"]\\+\\?\\)\"[^>]\\+\\?>\\([^<]\\+\\?\\)</a>";

    regex_t reg;
    regcomp(&reg,regstr,0);

    int fd = open("url.html",O_RDONLY);
    int size = lseek(fd,0,SEEK_END);

    char * mmap_data = NULL;
    mmap_data = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);

    int regnum = 3 ;
    regmatch_t match[regnum];
    char link[1024];
    char title[1024];

    while((regexec(&reg,mmap_data,regnum,match,0))==0)
    {
        bzero(link,sizeof(link));
        bzero(title,sizeof(title));
        snprintf(link, match[1].rm_eo-match[1].rm_so+1, "%s", mmap_data+match[1].rm_so);
        snprintf(title, match[2].rm_eo-match[2].rm_so+1, "%s", mmap_data+match[2].rm_so);

        mmap_data+=match[0].rm_eo;
        printf("匹配结果 ， title = %s    link = %s\n", title, link);
    }
    regfree(&reg);
    return 0;
}
