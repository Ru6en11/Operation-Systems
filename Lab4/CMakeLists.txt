cmake_minimum_required(VERSION 3.10)

project(App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /EHsc")
    set(CMAKE_CXX_FLAGS "/Wall /EHsc")
else()
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

add_executable(App main.cpp)

if (WIN32)
    add_executable(simulator simulator_win.cpp)
else()
    add_executable(simulator simulator.cpp)
endif()

target_include_directories(App PRIVATE lib)
