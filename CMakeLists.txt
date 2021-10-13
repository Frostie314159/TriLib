cmake_minimum_required(VERSION 3.21)

project(TriLibTests)

add_executable(TriLibTests Tests.cpp)

if(MSVC)
    target_compile_options(TriLibTests INTERFACE /arch:SSE3)
    target_compile_options(TriLibTests PUBLIC /Wall)
else()
    target_compile_options(TriLibTests PUBLIC -mavx -pedantic -fconcepts)
endif()

target_include_directories(TriLibTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)