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

add_executable(App 
  main.cpp 
  lib/background.cpp
  lib/counter.cpp
  lib/shared_memory.cpp
  lib/utils.cpp
)

target_include_directories(App PRIVATE lib)
