cmake_minimum_required(VERSION 3.13)

project(TriLibTests)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/FindSSE.cmake)

add_executable(TriLibTests ${CMAKE_CURRENT_SOURCE_DIR}/Tests.cpp)

if(MSVC)
    target_compile_options(TriLibTests PUBLIC /Wall)
else()
    target_compile_options(TriLibTests PUBLIC -msse2 -pedantic -fconcepts)
endif()


target_include_directories(TriLibTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)