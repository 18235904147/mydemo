//
// Created by sqp on 2022/6/22.
//
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include "tlv_box.h"
#include "unix-util.h"
#include "discunixserver.h"

using namespace tlv;

#define QLEN 10
#define BUF_LEN 1024

//const char *SERVER_UNIX_SOCKET_ADDR = "/tmp/kylin.deviceDiscover.socket";

//创建服务器进程，成功返回0，出错返回小于0的errno
//int server_listen()
//{
//    int fd;
//    struct sockaddr_un serverAddr;
//
//    //创建本地domain套接字
//    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
//        printf("server create socket error! errno = %d\n", errno);
//        return SOCKET_CREATE_ERROR;
//    }
//
//    //删除套接字文件，避免因文件存在导致bind()绑定失败
//    unlink(SERVER_UNIX_SOCKET_ADDR);
//
//    //初始化套接字结构体地址
//    memset(&serverAddr, 0, sizeof(serverAddr));
//    serverAddr.sun_family = AF_UNIX;
//    strcpy(serverAddr.sun_path, SERVER_UNIX_SOCKET_ADDR);
//
//    if (bind(fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
//        close(fd);
//        printf("server bind error!errno = %d\n", errno);
//        return BIND_ERROR;
//    }
//
//    if (listen(fd, QLEN) < 0) { //告知内核这是一个服务器进程
//        close(fd);
//        printf("server listen error! errno = %d\n", errno);
//        return LISTEN_ERROR;
//    }
//    return (fd);
//}
//
//int server_accept(int listenfd)
//{
//    int recvfd;
//    struct sockaddr_un acceptAddr;
//    socklen_t socklen = sizeof(acceptAddr);
//    if ((recvfd = accept(listenfd, (struct sockaddr *)&acceptAddr, &socklen)) < 0){
//        printf("Accept failed , errno = %d", errno);
//        return ACCEPT_ERROR;
//    }
//    //到此成功获取路径名
//    unlink(acceptAddr.sun_path);
//    return (recvfd);
//}
//
//int main(void)
//{
//    int server_fd, client_fd;
//    char buf[BUF_LEN] ;
//    if  ((server_fd = server_listen()) < 0){
//        return -1;
//    }
//
//    printf("start server... \n");
//    if((client_fd = server_accept(server_fd)) < 0){
//        return -1;
//    }
//
//
//    while (1) {
//        int len = read(client_fd, buf, BUF_LEN);
//
//        DiscoveryDeviceInfo parsedBoxes;
//        if (!parsedBoxes.Parse((unsigned char*)buf, BUF_LEN) ){
//            std::cout << "boxes Parse Failed !\n";
//            return -1;
//        }
//
//        std::cout << "boxes Parse Success, " << parsedBoxes.GetSerializedBytes() << " bytes \n";
//
//        DiscoveryDeviceInfo parsedBox;
//        if (!parsedBoxes.GetObjectValue(DEVICE_ALL, parsedBox)) {
//            std::cout << "GetObjectValue Failed !\n";
//            return -1;
//        }
//
//        std::cout << "box Parse Success, " << parsedBox.GetSerializedBytes() << " bytes \n";
//
//        std::vector<int> tlvList;
//        int numTlvs = parsedBox.GetTLVList(tlvList);
//        std::cout <<  "box contains " << numTlvs << " TLVs: \n";
//        for (int i=0;i<numTlvs; i++)
//            std::cout << "Tlv " << std::hex << tlvList[i] << "\n";
//
//
//        {
//            int value;
//            if (!parsedBox.GetDevTypeValue(DEVICE_TYPE, value)) {
//                std::cout << "GetIntValue Failed !\n";
//                return -1;
//            }
//            std::cout << "GetIntValue Success " << value << std::endl;
//        }
//
//        {
//            char value[128];
//            int length = 128;
//            if (!parsedBox.GetUuidValue(DEVICE_UUID, value, length)) {
//                std::cout << "GetStringValue Failed !\n";
//                return -1;
//            }
//            std::cout << "GetStringValue Success " << value << std::endl;
//        }
//
//        {
//            char value[128];
//            int length = 128;
//            if (!parsedBox.GetDevNameValue(DEVICE_NAME, value, length)) {
//                std::cout << "GetStringValue Failed !\n";
//                return -1;
//            }
//            std::cout << "GetStringValue Success " << value << std::endl;
//        }
//
//
//        if (len < 0 ) {
//            printf("eeeeeeeeeeeeeeeeeeee\n");
//            if (errno == EINTR){
//                continue;
//            }
//        } else if (len == 0) {
//            printf("the other side has been closed.\n");
//            break;
//        }
//        printf("reve::: %s", buf);
//    }
//    close(client_fd);
//    close(server_fd);
//    return 0;
//}

int main(){
    DiscUnixServer *server = new DiscUnixServer;
    server->startServer();
    server->recvDevInfo();
}