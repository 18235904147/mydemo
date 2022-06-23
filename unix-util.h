//
// Created by sqp on 2022/6/22.
//

#ifndef UNIX_SOCKET_DEMO_UNIX_UTIL_H
#define UNIX_SOCKET_DEMO_UNIX_UTIL_H

enum ErrorType{
    SOCKET_CREATE_ERROR = -1,
    BIND_ERROR = -2,
    LISTEN_ERROR = -3,
    ACCEPT_ERROR = -4,
    CONNECT_ERROR = -5,
    SEND_ERROR = -6,
    RECV_ERROR = -7
};


#endif //UNIX_SOCKET_DEMO_UNIX_UTIL_H
