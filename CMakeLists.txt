###
# TODO: Add different build types, don't forget to link different libs with it as well!
cmake_minimum_required(VERSION 2.6)
project(ElementGames)

### NOTE: This section should only be for debugging I think
IF (WIN32)
	add_definitions(-DSFML_STATIC -W0)
	add_definitions(-W0)
ELSE (WIN32)
	add_definitions(-pg -Wall -ggdb -std=c++11) # For Profiling
	set(CMAKE_SHARED_LINKER_FLAGS "-pg")
ENDIF (WIN32)
###

IF (WIN32)
    #include_directories(C:\Users\Douglas\Projects\MSVC2010x86\include\ C:\Users\Douglas\Projects\MSVC2010x86\include\bullet\ ${CMAKE_CURRENT_BINARY_DIR})
	#link_directories(C:\Users\Douglas\Projects\MSVC2010x86\lib\release\)
ELSE (WIN32)
	include_directories(/usr/local/include/ ${CMAKE_CURRENT_BINARY_DIR} /usr/include/lua5.2/)
	#link_directories(Dependencies/Linux/lib)
ENDIF (WIN32)

#add the other source dirs
add_subdirectory(Engine)
add_subdirectory(Applications)

IF (NOT WIN32)
    #add_subdirectory(Scripting)
    add_subdirectory(Server)
ENDIF (WIN32)
