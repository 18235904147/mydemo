//
// Created by sqp on 2022/6/23.
//

#ifndef UNIX_SOCKET_DEMO_DISCUNICXLIENT_H
#define UNIX_SOCKET_DEMO_DISCUNICXLIENT_H


class DiscUnixClient {

public:
    DiscUnixClient();
    ~DiscUnixClient();
    void startClient();
    void stopClient();
    int sendDevInfo(char *uuid, const int devType, char *deName);
    int recvDevInfo();
    int getClientFd();

private:
    int connectServer();

private:
    int m_clientFd;
};


#endif //UNIX_SOCKET_DEMO_DISCUNICXLIENT_H
