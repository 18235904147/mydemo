//
// Created by sqp on 2022/6/23.
//

#include "discunixserver.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include "unix-util.h"
#include "tlv_box.h"
using namespace tlv;

const int BUF_LEN = 1024;
const char* SERVER_UNIX_SOCKET_ADDR = "/tmp/kylin.deviceDiscover.socket";
#define QLEN 10

DiscUnixServer::DiscUnixServer() {}

DiscUnixServer::~DiscUnixServer() {}

int DiscUnixServer::serverListen() {
    struct sockaddr_un serverAddr;

    //创建本地domain套接字
    if ((m_serverFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        printf("server create socket error! errno = %d\n", errno);
        return SOCKET_CREATE_ERROR;
    }

    //删除套接字文件，避免因文件存在导致bind()绑定失败
    unlink(SERVER_UNIX_SOCKET_ADDR);

    //初始化套接字结构体地址
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, SERVER_UNIX_SOCKET_ADDR);

    if (bind(m_serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        close(m_serverFd);
        printf("server bind error!errno = %d\n", errno);
        return BIND_ERROR;
    }

    if (listen(m_serverFd, QLEN) < 0) { //告知内核这是一个服务器进程
        close(m_serverFd);
        printf("server listen error! errno = %d\n", errno);
        return LISTEN_ERROR;
    }
}

int DiscUnixServer::serverAccept()
{
    struct sockaddr_un acceptAddr;
    socklen_t socklen = sizeof(acceptAddr);
    if ((m_clientFd = accept(m_serverFd, (struct sockaddr *)&acceptAddr, &socklen)) < 0){
        printf("Accept failed , errno = %d", errno);
        return ACCEPT_ERROR;
    }
    //到此成功获取路径名
    unlink(acceptAddr.sun_path);
}

int DiscUnixServer::startServer()
{
    if  (serverListen() < 0){
        return -1;
    }

    printf("start server... \n");
    if(serverAccept() < 0){
        return -1;
    }
}

void DiscUnixServer::stopServer() {
    close(m_clientFd);
    close(m_serverFd);
}

int DiscUnixServer::sendDevInfo(char *uuid, const int devType, char *deName) {
    DiscoveryDeviceInfo deviceInfo;

    deviceInfo.PutUuidValue(DEVICE_UUID, uuid);
    deviceInfo.PutDevTypeValue(DEVICE_TYPE, devType);
    deviceInfo.PutDevNameValue(DEVICE_NAME, deName);

    if (!deviceInfo.Serialize()) {
        std::cout << "deviceInfo Serialize Failed !\n";
        return -1;
    }

    DiscoveryDeviceInfo deviceInfoAll;
    deviceInfoAll.PutObjectValue(DEVICE_ALL, deviceInfo);

    if (!deviceInfoAll.Serialize()) {
        std::cout << "deviceInfoAll Serialize Failed !\n";
        return -1;
    }


    if(send(m_clientFd, deviceInfoAll.GetSerializedBuffer(), deviceInfoAll.GetSerializedBytes(), 0) < 0){
        printf("send data fail!\n");
        return SEND_ERROR;
    }
}

int DiscUnixServer::recvDevInfo() {
    while (1) {
        char buf[BUF_LEN];

        sleep(1);
        int len = read(m_clientFd, buf, BUF_LEN);
        if (len < 0 ) {
            printf("recv device info fail\n");
            continue;
        } else if (len == 0) {
            printf("the other side has been closed.\n");
            break;
        }

        DiscoveryDeviceInfo parsedBoxes;
        if (!parsedBoxes.Parse((unsigned char*)buf, len)){
            std::cout << "boxes Parse Failed !\n";
            return -1;
        }

        std::cout << "boxes Parse Success, " << parsedBoxes.GetSerializedBytes() << " bytes \n";

        DiscoveryDeviceInfo parsedBox;
        if (!parsedBoxes.GetObjectValue(DEVICE_ALL, parsedBox)) {
            std::cout << "GetObjectValue Failed !\n";
            return -1;
        }

        std::cout << "box Parse Success, " << parsedBox.GetSerializedBytes() << " bytes \n";

        std::vector<int> tlvList;
        int numTlvs = parsedBox.GetTLVList(tlvList);
        std::cout <<  "box contains " << numTlvs << " TLVs: \n";
        for (int i=0;i<numTlvs; i++)
            std::cout << "Tlv " << std::hex << tlvList[i] << "\n";


        {
            int value;
            if (!parsedBox.GetDevTypeValue(DEVICE_TYPE, value)) {
                std::cout << "GetIntValue Failed !\n";
                return -1;
            }
            std::cout << "GetIntValue Success " << value << std::endl;
        }

        {
            char value[128];
            int length = 128;
            if (!parsedBox.GetUuidValue(DEVICE_UUID, value, length)) {
                std::cout << "GetStringValue Failed !\n";
                return -1;
            }
            std::cout << "GetStringValue Success " << value << std::endl;
        }

        {
            char value[128];
            int length = 128;
            if (!parsedBox.GetDevNameValue(DEVICE_NAME, value, length)) {
                std::cout << "GetStringValue Failed !\n";
                return -1;
            }
            std::cout << "GetStringValue Success " << value << std::endl;
        }

        printf("reve::: %s", buf);
    }
}
