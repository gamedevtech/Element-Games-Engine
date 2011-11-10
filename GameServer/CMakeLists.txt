cmake_minimum_required(VERSION 2.6)
project(EGServer)
add_executable(egserver main.cpp)
target_link_libraries(egserver sfml-system sfml-network)

