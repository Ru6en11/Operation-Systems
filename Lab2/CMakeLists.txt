cmake_minimum_required(VERSION 3.10)

project(TestApp LANGUAGES CXX)

if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /EHsc")
    set(CMAKE_CXX_FLAGS "/Wall /EHsc")
else()
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

add_executable(TestApp test.cpp lib/background.cpp)

target_include_directories(TestApp PRIVATE lib)
