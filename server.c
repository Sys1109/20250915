#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#define MAX_CLIENT  1024    // 最大支持客户端数量
#define BUF_SIZE    4096    // 数据缓冲区大小

/*网络初始化*/
int server_netinitializer(const char * ip , int port){
    int server_sockfd;
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    //网络信息结构体的初始化设置
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(!ip)
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        inet_pton(AF_INET,ip,&addr.sin_addr.s_addr);
    //sock create
    if((server_sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
        perror("sock create call failed");
    bind(server_sockfd,(struct sockaddr *)&addr,sizeof(addr));//使用自定义的网络信息对sockfd进行绑定
    listen(server_sockfd, 128); //设置开启监听，设置链接队列大小
    return server_sockfd;
}

//服务器启动接口
//客户端向服务器发送 time 关键字， 服务器向客户端返回系统时间，如果不是time ，服务器响应固定的 please tryagain..\n
//客户端连接成功后， 服务器向客户端发送连接信息 "hi ip connect successfully\n"
int server_start(int shutdown){
    //单进程非阻塞模型

    //网络初始化
    int server_sockfd = server_netinitializer(NULL, 8888);

    //server_sockfd设置，变为非阻塞sockfd
    int flag;
    flag = fcntl(server_sockfd, F_GETFL);   //获取server_sockfd的选项
    flag |= O_NONBLOCK;                     //修改成非阻塞选项
    fcntl(server_sockfd, F_SETFL, flag);    //将flag设置给server_sockfd，变为非阻塞socket

    //client_sockfd数组， 保存所有连接成功的sockfd, client_sock_array 初始化-1
    int client_sock_array[MAX_CLIENT];
    for(int i = 0; i < MAX_CLIENT; i++){
        client_sock_array[i] = -1;
    }

    socklen_t addrlen;   //网络信息结构体大小
    struct sockaddr_in client_addr;
    char response_buffer[BUF_SIZE];

    //循环等待连接即可
    while(1){
        //调用accept 查看是否有连接请求， 如果有， 建立连接，而后存储client_sock
        addrlen = sizeof(client_addr);
        int client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &addrlen);
        if(client_sockfd > 0){
            // 新客户端接入，将其socket也设为非阻塞
            int c_flag = fcntl(client_sockfd, F_GETFL);
            c_flag |= O_NONBLOCK;
            fcntl(client_sockfd, F_SETFL, c_flag);

            // 寻找空位置存入客户端数组
            int i;
            for(i = 0; i < MAX_CLIENT; i++){
                if(client_sock_array[i] == -1){
                    client_sock_array[i] = client_sockfd;
                    break;
                }
            }
            if(i == MAX_CLIENT){
                // 达到最大连接数，关闭新连接
                close(client_sockfd);
                continue;
            }

            // 连接成功，向客户端发送欢迎消息
            char welcome_buf[128];
            sprintf(welcome_buf, "hi %s connect successfully\n", inet_ntoa(client_addr.sin_addr));
            send(client_sockfd, welcome_buf, strlen(welcome_buf), 0);
        }
        else{
            // 非阻塞模式下无新连接，属于正常情况，仅打印真正的错误
            if(errno != EAGAIN && errno != EWOULDBLOCK){
                perror("accept call failed");
            }
        }

        //遍历client_sock数组， 读取每一个 sock，如果有请求数据则处理， 没有继续， 遍历到末尾
        for(int i = 0; i < MAX_CLIENT; i++){
            int sockfd = client_sock_array[i];
            if(sockfd == -1){
                continue;
            }

            memset(response_buffer, 0, BUF_SIZE);
            int recvlen = recv(sockfd, response_buffer, BUF_SIZE-1, MSG_DONTWAIT);

            if(recvlen > 0){
                //读取到数据， 处理
                // 去掉输入末尾的换行符，方便字符串匹配
                response_buffer[strcspn(response_buffer, "\r\n")] = '\0';

                if(strcmp(response_buffer, "time") == 0){
                    // 客户端输入time，返回系统时间
                    time_t t_now = time(NULL);
                    char *time_str = ctime(&t_now);
                    send(sockfd, time_str, strlen(time_str), 0);
                }
                else{
                    // 非time指令，返回固定提示
                    char *reply = "please tryagain..\n";
                    send(sockfd, reply, strlen(reply), 0);
                }
            }
            else if(recvlen < 0){
                if(errno == EAGAIN || errno == EWOULDBLOCK){
                    //recv 非阻塞返回，当前无数据可读
                    continue;
                }
                else{
                    perror("recv call failed");
                    close(sockfd);
                    client_sock_array[i] = -1; //复位
                }
            }
            else{
                //读取sockfd 返回0， 客户端断开
                close(sockfd);
                client_sock_array[i] = -1; //复位
            }
        }

        // 轮询间隔，避免空转占满CPU
        usleep(1000);
    }

    return 0;
}

int main(void){
    server_start(1); //启动
    return 0;
}
