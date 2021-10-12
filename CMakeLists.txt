cmake_minimum_required(VERSION 3.21)

project(TriLibTests)

add_executable(TriLibTests Tests.cpp)

if(MSVC)
    target_compile_options(TriLibTests INTERFACE /arch:SSE2)
else()
    target_compile_options(TriLibTests INTERFACE -msse2)
endif()
