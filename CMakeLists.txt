cmake_minimum_required(VERSION 3.31)
project(G192)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE vendor/g719/basicop vendor/g719/include)
file(GLOB_RECURSE G719_SOURCES vendor/g719/*.h vendor/g719/*.c)
target_sources(${PROJECT_NAME} PRIVATE ${G719_SOURCES} main.cpp)
