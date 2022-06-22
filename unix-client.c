//
// Created by sqp on 2022/6/22.
//

#include <stdio.h>
#include <stddef.h>s
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "unix-util.h"


#define BUF_LEN 1024

const char *SERVER_UNIX_SOCKET_ADDR = "/tmp/kylin.deviceDiscover.socket";

//创建客户端进程，成功返回0，出错返回小于0的errno
int connectServer()
{
    int fd;
    struct sockaddr_un clientAddr;
    //创建本地套接字domain
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        printf("server create socket error! errno = %d\n", errno);
        return SOCKET_CREATE_ERROR;
    }

    //使用服务器进程地址填充socket地址结构体
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sun_family = AF_UNIX;
    strcpy(clientAddr.sun_path, SERVER_UNIX_SOCKET_ADDR);
    if (connect(fd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0) {
        printf("client: connect failed! errno=%d\n",errno);
        close(fd);
        return CONNECT_ERROR;
    }
    return (fd);
}
int main(void)
{
    int fd;
    char buf[BUF_LEN];
    fd = connectServer();

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        write(fd, buf, strlen(buf));
    }
    close(fd);
    return 0;
}