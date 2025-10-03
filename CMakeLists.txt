cmake_minimum_required(VERSION 3.10)
project(injector)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_GENERATOR_PLATFORM Win32) to build for x32 
#x64 is default

include_directories(injector/source/include)

add_executable(injector 
    injector/source/src/main.cpp
    injector/source/src/inject.cpp
    injector/source/src/process.cpp
)

add_library(test SHARED testdll/test.cpp)
