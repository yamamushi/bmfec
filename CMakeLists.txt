cmake_minimum_required(VERSION 2.8)

project(BMFEC)
SET(BMFEC_VERSION 0.01)


find_package(Git)
if(GIT_FOUND)
    execute_process(COMMAND git "describe" OUTPUT_VARIABLE GIT_VERSION WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" VERSION_MAJOR "${GIT_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${GIT_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_BUGFIX "${GIT_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+-[0-9]+-(.*)" "\\1" VERSION_SHA1 "${GIT_VERSION}")
    if("${VERSION_SHA1}" STREQUAL "")
        set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}")
    else()
        set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}-${VERSION_SHA1}")
    endif()
else()
  message(FATAL_ERROR "Git not found.")
endif()


#-----------------------------------------------------------------------------
# Options for building

option(BMFEC_BUILD_VERBOSE       "Verbose build output." ON)

if(BMFEC_BUILD_VERBOSE)
  set(CMAKE_VERBOSE_MAKEFILE true)
endif()


#-----------------------------------------------------------------------------
# Print system information

message(STATUS "CMake version:   ${CMAKE_VERSION}")
message(STATUS "System:          ${CMAKE_SYSTEM}")
message(STATUS "Processor:       ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Verbose:         ${BMFEC_BUILD_VERBOSE}")


#-----------------------------------------------------------------------------
# Collect into bin or lib subdirectories

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    CACHE PATH "Single directory for all static libraries.")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    CACHE PATH "Single directory for all dynamic libraries on Unix.")


#-----------------------------------------------------------------------------
# Check which compiler to use

if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  if(HAVE_STD11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
  else()
    message(FATAL_ERROR "No advanced standard C++ support (-std=c++11 not defined) for GCC version.")
  endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  if(HAVE_STD11)
    if(APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall -Wextra -U__STRICT_ANSI__ -Wno-unused-parameter")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter")
    endif()
  else()
    message(FATAL_ERROR "No C++11 support for Clang version. Please upgrade Clang to a version supporting C++11.")
  endif()
endif()

message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Flags:       ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ link flags:  ${CMAKE_CXX_LINK_FLAGS}")


#-----------------------------------------------------------------------------
# System libraries used for linking.

if(UNIX AND NOT APPLE)
  list(APPEND BMFEC_SYSTEM_LIBRARIES rt)
  list(APPEND BMFEC_SYSTEM_LIBRARIES dl)
endif()


#-----------------------------------------------------------------------------
# Get submodules

execute_process(COMMAND git "submodule" "update" "--init" "--recursive" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


#-----------------------------------------------------------------------------
# Third party libraries

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(cmake/Modules/submodules.cmake)
include(CheckIncludeFiles)
include (FindPkgConfig)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_COMPONENTS system random filesystem program_options )
find_package(Boost 1.53 REQUIRED ${Boost_COMPONENTS})

find_package(XMLRPC REQUIRED c++2 libwww-client)

PKG_SEARCH_MODULE(SDL2 OPTIONAL sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE OPTIONAL SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF OPTIONAL SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER OPTIONAL SDL2_mixer>=2.0.0)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${SDL2MIXER_INCLUDE_DIR})

set(_BMFEC_SDLCLIENT_ ON BOOL "SDL Client") # Configurable by user
CHECK_INCLUDE_FILES("${SDL2_INCLUDE_DIRS}/SDL.h;${SDL2_INCLUDE_DIRS}/SDL_image.h;${SDL2_INCLUDE_DIRS}/SDL_ttf.h;${SDL2_INCLUDE_DIRS}/SDL_mixer.h" _BMFEC_SDLCLIENT_)


set(_BMFEC_NCURSESCLIENT_ ON BOOL "Ncurses Client") # Configurable by user


configure_file(src/config.h.in src/config.h)


#-----------------------------------------------------------------------------
# Force out-of-source build

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
message(FATAL_ERROR "CMake generation is not allowed within the source directory!
Remove the CMakeCache.txt file and try again from another folder, e.g.:

   rm CMakeCache.txt
   mkdir build
   cd build
   cmake ..
")
endif()


#-----------------------------------------------------------------------------
# Default to Release build

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_CURRENT_BINARY_DIR}/lib)

#-----------------------------------------------------------------------------


add_subdirectory(deps)
add_subdirectory(src)