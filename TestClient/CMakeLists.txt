cmake_minimum_required(VERSION 2.6)
project(EGTestClient)
add_executable(egtestclient main.cpp)
target_link_libraries(egtestclient sfml-system sfml-network)

