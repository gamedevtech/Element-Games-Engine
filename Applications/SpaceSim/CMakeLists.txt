cmake_minimum_required(VERSION 2.6)
add_subdirectory(Client)

IF (NOT WIN32)
    add_subdirectory(Server)
ENDIF (WIN32)
