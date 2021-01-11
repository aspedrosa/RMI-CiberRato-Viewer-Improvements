cmake_minimum_required(VERSION 3.17)
project(c_cpp)

set(CMAKE_CXX_STANDARD 20)

add_library(c_cpp structs.h DrawSock.cpp DrawSock.h cdrawlink.cpp cdrawlink.h netif.cpp netif.h)

#add_executable(c_cpp structs.h DrawSock.cpp DrawSock.h cdrawlink.cpp cdrawlink.h netif.cpp netif.h main.cpp)
