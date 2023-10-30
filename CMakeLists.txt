cmake_minimum_required(VERSION 3.26)
project(MB)

set(CMAKE_CXX_STANDARD 17)

# Set include dir
include_directories(./gtest/include)
include_directories(./gtest/include)

# Set Library dir
link_directories(./gtest/lib)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES CFG.cpp main.cpp
        Production.cpp
        Production.h
)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES
        CFG.cpp
        MBTests.cpp
        Production.cpp
)

# Create RELEASE target
add_executable(MB ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(MB_debug ${DEBUG_SOURCE_FILES} )

# Link library
target_link_libraries(MB_debug gtest)