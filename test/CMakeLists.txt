cmake_minimum_required(VERSION 3.14)
project(Connect4Tests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Add your test executable
add_executable(run_tests test.cpp) 

add_library(connect4lib ${src_files}) 
target_include_directories(connect4lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link your test executable with GoogleTest and your library
target_link_libraries(run_tests GTest::gtest GTest::gtest_main connect4lib) 

# Add test target 
add_test(NAME BoardTests COMMAND run_tests)