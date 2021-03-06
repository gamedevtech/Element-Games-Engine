cmake_minimum_required(VERSION 2.6)
project(ElementGames)

### NOTE: This section should only be for debugging I think
IF (WIN32)
    #NOTHING
ELSE (WIN32)
    add_definitions(-pg -Wall -ggdb) # For Profiling
    set(CMAKE_SHARED_LINKER_FLAGS "-pg")
ENDIF (WIN32)

IF (WIN32)
    IF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
        include_directories(../Dependencies/Windows/include ${CMAKE_CURRENT_BINARY_DIR})
        #link_directories(../../Dependencies/Windows/library/debug)
    ENDIF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
ELSE (WIN32)
    include_directories(/usr/local/include/ /usr/local/include/mongo/ ${CMAKE_CURRENT_BINARY_DIR})
    link_directories(../../../build/Engine)#../../Dependencies/linux/lib)
ENDIF (WIN32)

set(SRCS
    ./Networking.h
    ./Packet.h
    ./Database.h
    ./Server.h
    ./Source/Networking.cpp
    ./Source/Packet.cpp
    ./Source/Database.cpp
    ./Source/Server.cpp
)

IF (WIN32)
    add_library(egserver STATIC ${SRCS})
    target_link_libraries(egserver sfml-system sfml-network lua mongoclient mingw32 egserver sfml-system sfml-network lua mongoclient mingw32)
ELSE (WIN32)
    add_library(egserver SHARED ${SRCS})
    target_link_libraries(egserver)
ENDIF (WIN32)
