bmfec
=================

A cross-platform BitMessage client with [Forward Error Correction](http://en.wikipedia.org/wiki/Forward_error_correction) support. 

This client will compile with support for ncurses and SDL, and html/css support through [emscripten](http://en.wikipedia.org/wiki/Emscripten) is planned.


#Building

##Dependencies:

    Boost (There is no guarantee anything less than version 1.52 will work properly)
    GCC 4.7+ or Clang (Needs to be fully C++11 Compatible)
    Sqlite3
    xmlrpc-c
    pkgconfig


**Ncurses Mode**

This is currently the default mode built:

    Ncursesw

**SDL2 Mode**

    sdl2
    sdl2_image
    sdl2_mixer

**OS X Users** 

If you are on OSX using [homebrew](http://brew.sh/), you can install all of these dependencies with: 

    brew install ncurses sdl2 sdl2_mixer sdl2_image ncurses 
    brew install sqlite boost pkgconfig xmlrpc-c
    brew link ncurses --force



##Compiling

**Getting the source:**
    
    git clone https://github.com/yamamushi/bmfec
    cd bmfec
    git checkout develop

**Choosing a Client:**

You will need to edit this file:

     src/config.h.in

Uncomment the graphics mode you want to compile support for. Only choose one option, or -bad- things can happen. 

*There are currently plans to fix this to autodetect which you have support for, and to allow for compiling support for all rendering types.*


**Compiling:**

You will need to be in the project root directory to follow these directions (which would be warpdriveoverload if you followed the default instructions above).

    mkdir build
    cd build
    cmake .. && make
    bin/bmfec-client <password>


#Reporting Issues

Please open an [issue on Github](https://github.com/yamamushi/bmfec/issues), or contact the primary project owner at yamamushi [at] gmail [dot] com . We'll work as fast as possible to resolve any issues, as we want to maintain a good track record of maintenance for this project.
    

#Development:

Please contact yamamushi [at] gmail [dot] com if you'd like to contribute in any way, or just have general questions about the project. 




