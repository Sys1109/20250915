#Rource中的Makefile 负责代码的编译集成 生成ELF可执行文件

TARGET=process_copy
INCLUDE_PATH=../include/
LIBRARY_PATH=
#查找执行后缀文件
SRCFILE=$(wildcard *.c)
DESTFILE=$(patsubst %.c,%.o,$(SRCFILE))
CFLAGS=-I$(INCLUDE_PATH) -g -Wall
CC=gcc
#自底向上执行命令 自顶向下建立依赖关系


$(TARGET):$(DESTFILE)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $<
