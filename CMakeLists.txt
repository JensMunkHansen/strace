cmake_minimum_required(VERSION 3.0)

project(strace LANGUAGES CXX)

add_library(strace)
if (WIN32)
  target_sources(strace PRIVATE win32/strace.cpp)   
  target_include_directories(strace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/win32)
  add_library(libs::strace ALIAS strace)
  
  add_executable(strace_test win32/strace_test.cpp)
  target_link_libraries(strace_test libs::strace)
else()
  target_sources(strace PRIVATE strace.cpp)
  target_include_directories(strace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(strace PUBLIC dl) # Propagated to executable
  
  add_library(libs::strace ALIAS strace)
  
  add_executable(strace_test strace_test.cpp)
  target_link_libraries(strace_test libs::strace)
endif()
