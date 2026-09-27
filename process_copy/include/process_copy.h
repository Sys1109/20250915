#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

int check_pram(int ,const char* ,int);
int block_cur(const char* ,int);
int process_create(const char* ,const char* ,int ,int);
void process_wait(void);
