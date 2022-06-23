cmake_minimum_required(VERSION 3.16)
project(unix_socket_demo)

set(CMAKE_CXX_STANDARD 11)

add_executable(unix_server unix-server.cpp tlv.cpp tlv_box.cpp  discunixserver.cpp)

add_executable(unix-client unix-client.cpp  tlv.cpp tlv_box.cpp discunixclient.cpp)
