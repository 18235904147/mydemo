//
// Created by sqp on 2022/6/22.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include "unix-util.h"
#include "tlv_box.h"
#include "discunixclient.h"
using namespace tlv;

#define BUF_LEN 1024

//const char* SERVER_UNIX_SOCKET_ADDR = "/tmp/kylin.deviceDiscover.socket";
//
////创建客户端进程，成功返回0，出错返回小于0的errno
//int connectServer()
//{
//    int fd;
//    struct sockaddr_un clientAddr;
//    //创建本地套接字domain
//    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
//        printf("server create socket error! errno = %d\n", errno);
//        return SOCKET_CREATE_ERROR;
//    }
//
//    //使用服务器进程地址填充socket地址结构体
//    memset(&clientAddr, 0, sizeof(clientAddr));
//    clientAddr.sun_family = AF_UNIX;
//    strcpy(clientAddr.sun_path, SERVER_UNIX_SOCKET_ADDR);
//    if (connect(fd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0) {
//        printf("client: connect failed! errno=%d\n",errno);
//        close(fd);
//        return CONNECT_ERROR;
//    }
//    return (fd);
//}
int main(void)
{
//    int fd = connectServer();
//    DiscoveryDeviceInfo box;
//
//    box.PutUuidValue(DEVICE_UUID, (char *)"dfiu23yr8u2342390ds90fsdfa9s8f90");
//    box.PutDevTypeValue(DEVICE_TYPE, (int)3);
//    box.PutDevNameValue(DEVICE_NAME, (char *)"hello world !");
//
//    if (!box.Serialize()) {
//        std::cout << "box Serialize Failed !\n";
//        return -1;
//    }
//
//    std::cout << "box Serialize Success, " << box.GetSerializedBytes() << " bytes \n";
//
//    DiscoveryDeviceInfo boxes;
//    boxes.PutObjectValue(DEVICE_ALL, box);
//
//    if (!boxes.Serialize()) {
//        std::cout << "boxes Serialize Failed !\n";
//        return -1;
//    }
//
//    std::cout << "boxes Serialize Success, "  << boxes.GetSerializedBytes() << " bytes \n";
//    printf("%d", sizeof(boxes.GetSerializedBuffer()));
//
//
//    send(fd, boxes.GetSerializedBuffer(), boxes.GetSerializedBytes(), 0);
//
//
//    close(fd);

    DiscUnixClient *client = new DiscUnixClient;
    client->startClient();
    client->sendDevInfo("11111111111", 1, "sdasda");

    return 0;
}