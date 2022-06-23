cmake_minimum_required(VERSION 3.16)
project(unix_socket_demo C)

set(CMAKE_C_STANDARD 11)

add_executable(unix_server unix-server.c tlv.c)

add_executable(unix-client unix-client.c unix-util.h tlv.c tlv.h)
