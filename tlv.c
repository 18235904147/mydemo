//
// Created by sqp on 2022/6/23.
//

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tlv.h"


int send_tlv(int fd, unsigned char *buf, int buf_len, int status){
    tlv_t *send_tlv=NULL;
    int tag = fd;

    if (buf == NULL){
        printf("buffer is null!\n");
        return -1;
    }


    send_tlv = (tlv_t *)malloc(buf_len+TLV_TAG_LEN);
    memset(send_tlv,0,buf_len+TLV_TAG_LEN);

    //把数据整合到TLV结构中
    send_tlv->length = htonl(buf_len);
    send_tlv->tag = htonl(tag);
    send_tlv->value = (unsigned char*)send_tlv +TLV_TAG_LEN;
    memcpy(send_tlv->value,buf,buf_len);

    //发送TLV结构
    send(fd, (char *)send_tlv, sizeof(struct Message)+TLV_TAG_LEN,status);

    if (send_tlv != NULL){
        free(send_tlv);
    }

    return 0;
}

int recv_tlv(int fd, unsigned char *buf, int buf_len, int status){
    tlv_t send_tlv = {0, 0, NULL};
    int onece = 0, dataNum = 0;
    char tmp_tlv[1024];

    if (buf == NULL){
        printf("buffer is null!\n");
        return -1;
    }

    //获取TLV数据结构头
    onece = recv(fd, tmp_tlv, TLV_TAG_LEN, status);
    memcpy(&send_tlv.tag, tmp_tlv, 4);
    send_tlv.tag = ntohl(send_tlv.tag);
    memcpy(&send_tlv.length, tmp_tlv+4, 4);
    send_tlv.length = ntohl(send_tlv.length);
    send_tlv.value = (unsigned char *) malloc(send_tlv.length);

    //根据头数据接收后续value数据
    while (dataNum < send_tlv.length){
        onece = recv(fd, tmp_tlv, 1024, status);
        memcpy(send_tlv.value + dataNum, tmp_tlv, onece);
        dataNum += onece;
    }

    if(buf_len < send_tlv.length){
        printf("buffer overflow!\n");
        return -1;
    }

    memcpy(buf, send_tlv.value, send_tlv.length);
    if (send_tlv.value != NULL){
        free(send_tlv.value);
    }

    return dataNum;
}