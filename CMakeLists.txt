###
# TODO: Add different build types, don't forget to link different libs with it as well!
cmake_minimum_required(VERSION 2.6)
project(ElementGames)

### NOTE: This section should only be for debugging I think
IF (WIN32)
	add_definitions(-DSFML_DYNAMIC -Wall)
ELSE (WIN32)
	add_definitions(-pg -Wall -ggdb) # For Profiling
	set(CMAKE_SHARED_LINKER_FLAGS "-pg")
ENDIF (WIN32)
###

IF (WIN32)
	IF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
                #include_directories(Dependencies/Windows/include ${CMAKE_CURRENT_BINARY_DIR})
		#link_directories(Dependencies/Windows/library/debug)
	ENDIF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
ELSE (WIN32)
	include_directories(/usr/local/include/ ${CMAKE_CURRENT_BINARY_DIR})
	#link_directories(Dependencies/Linux/lib)
ENDIF (WIN32)

#add the other source dirs
add_subdirectory(Engine)
add_subdirectory(Applications)

IF (WIN32)
    add_subdirectory(Scripting)
ENDIF (WIN32)
