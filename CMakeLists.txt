cmake_minimum_required(VERSION 3.20)
project(grafos_parte2)

set(CMAKE_CXX_STANDARD 11)

add_executable(grafos_parte2 main.cpp Graph.h Graph.cpp Node.h Node.cpp Edge.h Edge.cpp Cluster.cpp Cluster.h)
