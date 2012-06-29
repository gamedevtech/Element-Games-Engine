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
		include_directories(../../../Dependencies/Windows/include ${CMAKE_CURRENT_BINARY_DIR})
		#link_directories(../../Dependencies/Windows/library/debug)
	ENDIF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
ELSE (WIN32)
	include_directories(/usr/local/include/ ${CMAKE_CURRENT_BINARY_DIR})
	link_directories(../../../build/Engine)#../../Dependencies/linux/lib)
ENDIF (WIN32)

add_executable(element_games_model_converter ./main.cpp ./ModelConverter.cpp)
IF (WIN32)
	#NOTHING
ELSE (WIN32)
	set_target_properties(element_games_model_converter PROPERTIES LINK_FLAGS -pg)
ENDIF (WIN32)

IF (WIN32)
	IF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
                target_link_libraries(element_games_model_converter egengine glew_static glut32 glu32 opengl32 sfml-system sfml-audio sfml-network sfml-window sfml-graphics assimp BulletDynamics BulletCollision LinearMath Awesomium freetype ftgl mingw32 egengine glew_static glut32 glu32 opengl32 sfml-system sfml-audio sfml-network sfml-window sfml-graphics assimp BulletDynamics BulletCollision LinearMath Awesomium freetype ftgl mingw32)
	ENDIF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
	IF (CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")
		target_link_libraries(element_games_model_converter egengine glew32s glut32 glu32 opengl32 sfml-system sfml-audio sfml-network sfml-window sfml-graphics assimp BulletDynamics BulletCollision LinearMath Awesomium freetype246 ftgl_static)
	ENDIF (CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")
ELSE (WIN32)
    target_link_libraries(element_games_model_converter egengine sfml-graphics sfml-window sfml-system sfml-audio sfml-network assimp GLEW glut GLU BulletDynamics BulletCollision LinearMath Xrandr ftgl awesomium-1.6.5)
ENDIF (WIN32)
