cmake_minimum_required(VERSION 2.6)
project(ElementGames)

### NOTE: This section should only be for debugging I think
IF (WIN32)
	add_definitions(-DSFML_STATIC -W0)
ELSE (WIN32)
	add_definitions(-pg -Wall -ggdb) # For Profiling
	set(CMAKE_SHARED_LINKER_FLAGS "-pg")
ENDIF (WIN32)

IF (WIN32)
	IF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
		include_directories(../../../Dependencies/Windows/include ${CMAKE_CURRENT_BINARY_DIR})
		#link_directories(../../Dependencies/Windows/library/debug)
	ENDIF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
ELSE (WIN32)
	include_directories(/usr/local/include/ ${CMAKE_CURRENT_BINARY_DIR})
	link_directories(../../../build/Engine)#../../Dependencies/linux/lib)
ENDIF (WIN32)

add_executable(element_games_sandbox ./Sandbox.h ./Sandbox.cpp ./main.cpp)
IF (WIN32)
	#NOTHING
ELSE (WIN32)
	set_target_properties(element_games_sandbox PROPERTIES LINK_FLAGS -pg)
ENDIF (WIN32)

IF (WIN32)
	target_link_libraries(element_games_sandbox egengine glew32 glut32 glu32 opengl32 sfml-audio-s sfml-network-s sfml-graphics-s sfml-window-s sfml-system-s assimp BulletDynamics BulletCollision LinearMath Awesomium lua freetype ftgl)
ELSE (WIN32)
target_link_libraries(element_games_sandbox egengine sfml-graphics sfml-window sfml-system sfml-audio sfml-network assimp GL GLEW glut GLU BulletDynamics BulletCollision LinearMath Xrandr ftgl awesomium-1.6.5 lua5.2)
ENDIF (WIN32)
