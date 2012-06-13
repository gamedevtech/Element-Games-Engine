For the raw dependencies sources, you can download them as a zip file from here:
www.palodequeso.net/Dependencies.zip

If you are building this on a 64 bit windows platform. I have provided the prebuilt libraries as well as headers and binaries required to built this project yourself with MinGW. I normally do my development under Linux but seeing how this project is designed to be cross platform, I might as well make it easy to write and compile this code everywhere.
www.palodequeso.net/Win64MinGW.zip
^-- Out of date... sorry, need to rebuild boost in windows with -fPIC as described below.

Bullet must be built with -fPIC for python compatability
To do so, you must run cmake -G "Some Generator" I like "Unix Makefiles"
and then edit CMakeCache.txt to say CMAKE_CXX_FLAGS:STRING=-fPIC as well as the CMAKE_C_FLAGS
Information Found here!
http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?t=5250
