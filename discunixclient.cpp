//
// Created by sqp on 2022/6/23.
//

#include "discunixclient.h"

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

DiscUnixClient::DiscUnixClient() {}

DiscUnixClient::~DiscUnixClient() {}

int DiscUnixClient::connectServer()
{
    struct sockaddr_un clientAddr;
    //创建本地套接字domain
    if ((m_clientFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        printf("server create socket error! errno = %d\n", errno);
        return SOCKET_CREATE_ERROR;
    }

    //使用服务器进程地址填充socket地址结构体
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sun_family = AF_UNIX;
    strcpy(clientAddr.sun_path, SERVER_UNIX_SOCKET_ADDR);
    if (connect(m_clientFd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0) {
        printf("client: connect failed! errno=%d\n",errno);
        close(m_clientFd);
        return CONNECT_ERROR;
    }
}

void DiscUnixClient::startClient()
{
    connectServer();
}

void DiscUnixClient::stopClient() {
    close(m_clientFd);
}

int DiscUnixClient::sendDevInfo( char *uuid, const int devType, char *deName) {
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


    printf("send data %d!\n" ,deviceInfoAll.GetSerializedBytes());
    if(send(m_clientFd, deviceInfoAll.GetSerializedBuffer(), deviceInfoAll.GetSerializedBytes(), 0) < 0){
        printf("send data fail!\n");
        return SEND_ERROR;
    }
}

int DiscUnixClient::recvDevInfo() {
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
        if (!parsedBoxes.Parse((unsigned char*)buf, BUF_LEN) ){
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

int DiscUnixClient::getClientFd() {
    return m_clientFd;
}
