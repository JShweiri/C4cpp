cmake_minimum_required(VERSION 3.10)
project(Connect4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -O3")

# DEBUG build settings (cmake -DCMAKE_BUILD_TYPE=Debug ..):
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -g -O0")
  message(STATUS "Using Debug build settings." )
endif()

set(CMAKE_CXX_STANDARD 17)

file(GLOB src_files src/*.cpp)
add_executable(Connect4 ${src_files})

include_directories(include) 

target_link_libraries(Connect4)

add_subdirectory(test)
