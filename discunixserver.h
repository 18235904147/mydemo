//
// Created by sqp on 2022/6/23.
//

#ifndef UNIX_SOCKET_DEMO_DISCUNIXSERVER_H
#define UNIX_SOCKET_DEMO_DISCUNIXSERVER_H


class DiscUnixServer {

public:
    DiscUnixServer();
    ~DiscUnixServer();
    int startServer();
    void stopServer();
    int sendDevInfo(char *uuid, const int devType, char *deName);
    int recvDevInfo();

private:
    int serverListen();
    int serverAccept();

private:
    int m_serverFd;
    int m_clientFd;
};


#endif //UNIX_SOCKET_DEMO_DISCUNIXSERVER_H
