//
// Created by sqp on 2022/6/23.
//

#ifndef UNIX_SOCKET_DEMO_TLV_H
#define UNIX_SOCKET_DEMO_TLV_H
#include <stdio.h>
#include <string.h>

#define TLV_TAG_LEN 10

typedef struct _tlv {
    int tag;
    int length;
    unsigned char *value;
} tlv_t;


struct Message{
    unsigned int id;
    long num;
    char name[128];
    char hello[24];
    char new[128];
};


int recv_tlv(int fd, unsigned char *buf, int buf_len, int status);
int send_tlv(int fd, unsigned char *buf, int buf_len, int status);
#endif //UNIX_SOCKET_DEMO_TLV_H
