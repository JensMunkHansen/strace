cmake_minimum_required(VERSION 3.0)

project(strace LANGUAGES CXX)

add_library(strace)
target_sources(strace PRIVATE strace.cpp)
target_include_directories(strace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(strace PUBLIC dl) # Propagted to executable

add_library(libs::strace ALIAS strace)

add_executable(strace_test strace_test.cpp)
target_link_libraries(strace_test libs::strace)
