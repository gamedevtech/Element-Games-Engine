<!--For the raw dependencies sources, you can download them as a zip file from here:
www.palodequeso.net/Dependencies.zip

If you are building this on a 64 bit windows platform. I have provided the prebuilt libraries as well as headers and binaries required to built this project yourself with MinGW. I normally do my development under Linux but seeing how this project is designed to be cross platform, I might as well make it easy to write and compile this code everywhere.
www.palodequeso.net/Win64MinGW.zip
^-- Out of date... sorry, need to rebuild boost in windows with -fPIC as described below.

Bullet must be built with -fPIC for python compatability
To do so, you must run cmake -G "Some Generator" I like "Unix Makefiles"
and then edit CMakeCache.txt to say CMAKE_CXX_FLAGS:STRING=-fPIC as well as the CMAKE_C_FLAGS
Information Found here!
http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=5250-->


#Setting up Development Environment for Ubuntu 12.04:#
##Dependency Installation##
```sudo apt-get install git build-essential cmake libsndfile1-dev libopenal-dev libfreetype6-dev libxrandr-dev libx11-dev libboost-dev libboost-python-dev libglu1-mesa-dev libxmu-dev libxi-dev freeglut3-dev libjpeg-dev libjpeg62-dev libboost-filesystem-dev libboost-system-dev libreadline-dev libncurses5-dev scons libboost-thread-dev libboost-program-options-dev```

##I also like to use KDevelop, but it's up to you!##
```sudo apt-get install kdevelop kdevelop-dev```

##While that's going, you can grab the extra dependencies source zip here##
[Dependencies.zip](http://www.palodequeso.net/Dependencies.zip)

###You should install the dependencies in this order:###
####assimp:####
```
cmake -G "Unix Makefiles"  
make  
sudo make install
```
####glew:####
```
make  
sudo make install  
sudo cp /usr/lib64/libGLEW.* /usr/lib/
```
####glm:####
```
sudo cp -R glm /usr/local/include/
```
####ftgl:####
```
sudo cp -R /usr/include/freetype2/freetype /usr/include/  
./configure  
make  
sudo make install
```
####sfml:####
```
cmake -G "Unix Makefiles"  
make  
sudo make install
```
###bullet:###
```
cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC"   
make  
sudo make install
sudo cp -R /usr/local/include/bullet/* /usr/local/include/
```
###awesomium###
```
sudo cp -R build/bin/release/* /usr/local/lib/  
sudo cp -R build/bin/release/* /usr/local/bin/  
sudo cp -R include/* /usr/local/include/
```
###lua###
```
make linux  
sudo make install
```
###mongod SERVER ONLY###
```
scons  
sudo scons install
```
###boost-python DEPRECATED###
```
Edit /usr/include/boost/python/detail/wrap_python.hpp as sudo  
replace "#include <pyconfig.h>" with "#include <python2.7/pyconfig.h>"  
replace "#include <patchlevel.h>" with "#include <python2.7/patchlevel.h>"  
replace "#include <Python.h>" with "#include <python2.7/Python.h>"
```
##Compile and Run!##

###Compile###
**with make**   
```
cmake -G "Unix Makefiles"  
make
```

**with kdevelop**  
```
Open Kdevelop  
in the menu click "Project -> Open / Import Project..."  
Select the Repo's CMakeLists.txt file  
click Finish  
Next I like the move the build directory out of the Repository but it's up to you!  
Click OK  
click Build Selection
```

###Run###
```
From the repository root  
<path_to_build_dir>/Applications/Testing/Sandbox/element_games_sandbox
```

*That's It!*  
  
**********************************************************************  
  
#Setting up the Build Environment for Windows 7 64bit with DevKit#
  
Because compiling and installing the dependencies on windows has been such a pain, I realized I could just distribute pre-compiled libraries, includes and bins for at least the windows system I have which is 64bit Windows 7  

For your convenience I've provided a Windows Installers zip file  
[WindowsInstallers.zip](http://www.palodequeso.net/WindowsInstallersQT.zip)  
You will also need the Win64bit Dev Kit Zip File
[Windows64DevKit.zip](http://www.palodequeso.net/Win64MinGW.zip)  

##First for the installers##
```
Unzip WindowsInstallersQT zip package somewhere  
Install CMake  
Install QT SDK (Run As Administrator)  
Run the OpenAL Installer
```

##Dependencies##
```
Unzip Win64MinGW.zip  
Copy the contents (bin, lib, include) to C:\QtSDK\mingw\
```

##Compile and Run##
```
Open the CMakeLists.txt from the root of Element-Games-Engine in QTCreator  
Click Build  
You can either run the exe from the Element-Games-Engine dir  
or Setup a run config in QtCreator
```

*Still need to include instructions on getting the python wrapper building, but the rest of it should build fine so far, it is for me*

<!--
**********************************************************************  
  
#Setting up the Build Environment for Windows 7 64bit by compiling libs#

This is pretty much the same process except instead of easily apt-getting some stuff you actually have to download the packages and compile/install them yourself.  

##Dependency Installation##

For your convenience I've provided a Windows Installers zip file  
[WindowsInstallers.zip](http://www.palodequeso.net/WindowsInstallersQT.zip)  
Also be sure to download the Dependencies zip file
[Dependencies.zip](http://www.palodequeso.net/Dependencies.zip)
  
Make sure as you're installing you're adding the bin dirs of these packages to your windows environment path!  

```
Unzip this zip package somewhere  
Install CMake  
Install QT SDK (Run As Administrator)  
Run the OpenAL Installer  
Install Code Blocks (you'll see why later)  
Install Awesomium  
NOTE: Boost is included in here and is needed for the normal dependency builds below  
```

###You should install the dependencies in this order:###

###boost###
```
unzip boost package  
inside boost_1_49_0 copy boost dir (includes) to C:\QtSDK\mingw\include\
```

###assimp###
```
Edit CMakeLists.txt in assimp dir  
set BUILD_ASSIMP_TOOLS to OFF  
set ENABLE_BOOST_WORKAROUND to ON  
Open QTCreator  
Open assimps CMakeLists.txt with (Open Project...)   
click the Hammer in the bottom left (build) wait til completion  
in C:\QtSDK\mingw\include create a assimp directory  
copy the contents of assimps include dir into that assimp directory  
then from the build dir that qtcreator generated (often one level up)  
enter the code directory  
copy libassimp.dll.a to C:\QtSDK\mingw\lib  
copy libassimp.dll to C:\QtSDK\mingw\bin
```

###glew###
```
This only comes with a vc6 project, which code blocks can import  
we use code blocks instead of vc6 because it can compile the vc project with mingw  
open code blocks  
file->import->MS Visual C++ Project  
select glew/build/glew_static  
click the gear icon in the toolbar to build  
it might seem like it failed but it should be fine  
copy glew-1.7.0\lib\libglew_static.a to C:\QtSDK\mingw\lib\  
copy glew-1.7.0\include\GL\* to C:\QtSDK\mingw\include\GL\
```

###glm###
```
cp glm-0.9.3.0\glm C:\QtSDK\mingw\include\
```

###Freetype###
```
open code blocks  
file->import->MS Visual C++ Project  
Select freetype-2.4.6\builds\win32\visualc\freetype  
build  
copy contents of freetype-2.4.6\include to C:\QtSDK\mingw\include\  
copy freetype-2.4.6\objs\release\ftdebug.o to C:\QtSDK\mingw\lib  
copy freetype-2.4.6\objs\release\libfreetype.a to C:\QtSDK\mingw\lib
```

###SFML###
```
with QtCreator open LaurentGomilla-SFML-*\CMakeLists.txt  
copy qtcreator-build\*.a to C:\QtSDK\mingw\lib  
copy qtcreator-build\*.dll to C:\QtSDK\mingw\bin  
copy LaurentGomilla-SFML-*\include\SFML to C:\QtSDK\mingw\include
```

###FTGL###
```
Open Code Blocks  
file->import->MS Visual C++ Project  
Select ftgl-2.1.3~rc5\msvc\vc6.1\ftgl_dll.vsproj  
select Build Target = Release Win32  
cp C:\QtSDK\mingw\lib\libfreetype.a to C:\QtSDK\mingw\lib\libfreetype234.a  
click build  
cp ftgl-2.1.3~rc5\msvc\Build\ftgl.dll C:\QtSDK\mingw\bin  
cp ftgl-2.1.3~rc5\msvc\Build\libftgl.dll.a C:\QtSDK\mingw\lib  
cp ftgl-2.1.3~rc5\src\FTGL C:\QtSDK\mingw\include\
```

###GLUT & Bullet###
```
cp bullet-2.79\Glut\GL\glut.h C:\QtSDK\mingw\include\GL\  
cp bullet-2.79\Glut\glut*.lib C:\QtSDK\mingw\lib\  
with QtCreator open bullet-2.79\CMakeLists.txt  
Build it  
It will appear to fail, but it just failed at building the tests, no biggie  
copy the contents of bullet-2.79\src\ to C:\QtSDK\mingw\include\  
copy the contents of bullet-2.79\lib\ to C:\QtSDK\mingw\lib\  
```

###Awesomium###
```
Run the Installer included in the Windows Package  
cp C:\Program Files (x86)\Khrona LLC\Awesomium SDK\1.6.6\build\include\Awesomium to C:\QtSDK\mingw\include\  
cp C:\Program Files (x86)\Khrona LLC\Awesomium SDK\1.6.6\build\lib\release\Awesomium.lib to C:\QtSDK\mingw\lib\  
copy contents of C:\Program Files (x86)\Khrona LLC\Awesomium SDK\1.6.6\build\bin\release\ to C:\QtSDK\mingw\bin\  
```

##Compile and Run##

Open CMakeLists.txt with QtCreator  
click Build  
You can either run the exe from the Element-Games-Engine dir  
or Setup a run config in QtCreator

*Still need to include instructions on getting the python wrapper building, but the rest of it should build fine so far, it is for me*
-->
