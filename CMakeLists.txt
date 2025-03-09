cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(general
  VERSION 1.0.0
  DESCRIPTION "General"
  LANGUAGES C CXX
)

# Enable verbose makefiles for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

# Ensure a default build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Unit test option
option(UNITTEST "UNITTEST enable" OFF)

# FetchContent allows pulling in external repos
include(FetchContent)

# Add custom cmake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Threads
find_package(Threads REQUIRED)

# Fetch nlohmann/json
FetchContent_Declare(
  json
  GIT_REPOSITORY "https://github.com/nlohmann/json.git"
  GIT_TAG "v3.11.2"
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(json)
message(STATUS "JSON fetched")

# Fetch GoogleTest if UNITTEST is on
if(UNITTEST)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "main"
  )
  FetchContent_MakeAvailable(googletest)
  message(STATUS "GTEST fetched")
endif()

# Build library sources
add_subdirectory(src)

# Add test subdirectory when enabled
if(UNITTEST)
  add_subdirectory(unittest)
endif()
