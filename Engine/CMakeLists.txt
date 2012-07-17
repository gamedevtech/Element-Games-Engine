cmake_minimum_required(VERSION 2.6)
IF (WIN32)
	IF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
		include_directories(../Dependencies/Windows/include/)
	ENDIF (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
ELSE (WIN32)
	include_directories(/usr/local/include/)
ENDIF (WIN32)

set(SRCS

	# Dynamics
	./Dynamics/Animation.h
	./Dynamics/BulletInterface.h
	./Dynamics/Physics.h
	./Dynamics/PhysicsSkeleton.h
	./Dynamics/Skeleton.h
	./Dynamics/Source/Animation.cpp
	./Dynamics/Source/BulletInterface.cpp
	./Dynamics/Source/Physics.cpp
	./Dynamics/Source/PhysicsSkeleton.cpp
	./Dynamics/Source/Skeleton.cpp

	# Game
	./Game/AI.h
	./Game/Object.h
	./Game/ObjectAttribute.h
	./Game/ObjectBasicAttribute.h
	./Game/ObjectControlAttribute.h
	./Game/ObjectEmissionAttribute.h
	./Game/ObjectRenderingAttribute.h
	./Game/ObjectManager.h
	./Game/Scene.h
	./Game/Game.h
	./Game/ObjectScript.h
	./Game/Source/AI.cpp
	./Game/Source/Object.cpp
	./Game/Source/ObjectAttribute.cpp
	./Game/Source/ObjectBasicAttribute.cpp
	./Game/Source/ObjectControlAttribute.cpp
	./Game/Source/ObjectEmissionAttribute.cpp
	./Game/Source/ObjectRenderingAttribute.cpp
	./Game/Source/ObjectManager.cpp
	./Game/Source/Scene.cpp
	./Game/Source/Game.cpp
	./Game/Source/ObjectScript.cpp

	# Graphics
	./Graphics/Camera.h
	./Graphics/CameraManager.h
	./Graphics/GraphicsSubsystem.h
	./Graphics/Light.h
	./Graphics/Mesh.h
	./Graphics/MeshGenerator.h
	./Graphics/MeshManager.h
	./Graphics/OpenGLInterface.h
	./Graphics/Renderer.h
	./Graphics/RendererDeferred.h
	./Graphics/RendererMultipass.h
	./Graphics/RenderingMaterial.h
	./Graphics/Shader.h
	./Graphics/ShaderManager.h
	./Graphics/Texture.h
	./Graphics/TextureManager.h
	./Graphics/OffscreenBuffer.h
	./Graphics/Particle.h
	./Graphics/Source/Camera.cpp
	./Graphics/Source/CameraManager.cpp
	./Graphics/Source/GraphicsSubsystem.cpp
	./Graphics/Source/Light.cpp
	./Graphics/Source/Mesh.cpp
	./Graphics/Source/MeshGenerator.cpp
	./Graphics/Source/MeshManager.cpp
	./Graphics/Source/OpenGLInterface.cpp
	./Graphics/Source/Renderer.cpp
	./Graphics/Source/RendererDeferred.cpp
	./Graphics/Source/RendererMultipass.cpp
	./Graphics/Source/RenderingMaterial.cpp
	./Graphics/Source/Shader.cpp
	./Graphics/Source/ShaderManager.cpp
	./Graphics/Source/Texture.cpp
	./Graphics/Source/TextureManager.cpp
	./Graphics/Source/OffscreenBuffer.cpp
	./Graphics/Source/Particle.cpp

	# Input
	./Input/Input.h
	./Input/Source/Input.cpp

	# Math
	#./Math/Vector3f.h
	#./Math/Vector4f.h
	#./Math/Matrix3f.h
	#./Math/Matrix4f.h
	#./Math/Transformationf.h
	./Math/Noise.h
	#./Math/Source/Vector3f.cpp
	#./Math/Source/Vector4f.cpp
	#./Math/Source/Matrix3f.cpp
	#./Math/Source/Matrix4f.cpp
	#./Math/Source/Transformationf.cpp
	./Math/Source/Noise.cpp
	./Math/Math.h
	./Math/Source/Math.cpp

	# Media
	./Media/AssimpInterface.h
	./Media/Image.h
	./Media/ModelLoader.h
	./Media/SFMLInterface.h
	./Media/ObjectWriter.h
	./Media/ObjectReader.h
	./Media/Source/AssimpInterface.cpp
	./Media/Source/Image.cpp
	./Media/Source/ModelLoader.cpp
	./Media/Source/SFMLInterface.cpp
	./Media/Source/ObjectWriter.cpp
	./Media/Source/ObjectReader.cpp

	# Networking
	./Networking/Networking.h
	./Networking/Packet.h
	./Networking/Source/Networking.cpp
	./Networking/Source/Packet.cpp

	# Utility
	./Utility/VariableManager.h
	./Utility/Window.h
	./Utility/Time.h
	./Utility/Font.h
	./Utility/Text.h
	./Utility/JSON.h
	./Utility/GUI.h
	./Utility/Dictionary.h
	./Utility/StringDictionary.h
	./Utility/UnsignedIntDictionary.h
	./Utility/StringMethods.h
	./Utility/Source/VariableManager.cpp
	./Utility/Source/Window.cpp
	./Utility/Source/Time.cpp
	./Utility/Source/Font.cpp
	./Utility/Source/Text.cpp
	./Utility/Source/GUI.cpp
	./Utility/Source/StringMethods.cpp
)

IF (WIN32)
    add_library(egengine STATIC ${SRCS})
    target_link_libraries(egengine glew_static glut32 glu32 opengl32 sfml-system sfml-audio sfml-network sfml-window sfml-graphics assimp BulletDynamics BulletCollision LinearMath Awesomium freetype ftgl_static mingw32 egengine glew_static glut32 glu32 opengl32 sfml-system sfml-audio sfml-network sfml-window sfml-graphics assimp BulletDynamics BulletCollision LinearMath Awesomium freetype ftgl_static mingw32)
ELSE (WIN32)
    add_library(egengine SHARED ${SRCS})
    target_link_libraries(egengine)
ENDIF (WIN32)
