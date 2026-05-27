cmake_minimum_required(VERSION 3.14.0)

project(Capstone)

add_executable(Capstone
  src/Main.cpp
  src/print.cpp
)

target_include_directories(Capstone
  PRIVATE include
)

# use this to link exteranl libraries
#target_link_libraries(Capstone
# PUBLIC libraryName
#)
