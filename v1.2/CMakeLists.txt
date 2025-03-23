cmake_minimum_required(VERSION 3.10)
project(cp_helper)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific definitions
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Find required packages
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# Add executable
add_executable(gen parse.cpp)

# Link libraries
target_link_libraries(gen ${CURL_LIBRARIES})

# Install target
install(TARGETS gen DESTINATION bin)
