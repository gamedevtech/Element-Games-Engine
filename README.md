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
```sudo apt-get install git build-essential cmake libsndfile1-dev libopenal-dev libfreetype6-dev libxrandr-dev libx11-dev libboost-dev libboost-python-dev libglu1-mesa-dev libxmu-dev libxi-dev freeglut3-dev```

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
cmake -G "Unix Makefiles"  
Edit CMakeCache.txt  
    Find CMAKE_CXX_FLAGS:STRING= and append -fPIC such that it reads "CMAKE_CXX_FLAGS:STRING=-fPIC"  
    Find CMAKE_C_FLAGS:STRING= and append -fPIC such that it reads "CMAKE_C_FLAGS:STRING=-fPIC"  
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
###boost-python###
```
Edit /usr/include/boost/python/detail/wrap_python.hpp as sudo  
replace "#include <pyconfig.h>" with "#include <python2.7/pyconfig.h>"  
replace "#include <patchlevel.h>" with "#include <python2.7/patchleve.h>"  
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
