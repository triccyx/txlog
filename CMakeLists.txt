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
option(UNITTEST "UNITTEST enable" ON)
option(COVERAGE "Coverage enable" ON)

if(COVERAGE)
    message("********Coverage enabled for project ${PROJECT_NAME}********")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage")
endif()

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

# Fetch magic_enum
if(CMAKE_CXX_STANDARD GREATER_EQUAL 17)
  FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.8.0
  )
  set(MAGIC_ENUM_OPT_BUILD_EXAMPLES OFF)
  set(MAGIC_ENUM_OPT_BUILD_TESTS OFF)
  FetchContent_MakeAvailable(magic_enum)
  message(STATUS "Fetch magic_enum")
endif()

# Build library sources
add_subdirectory(src)

# Add test subdirectory when enabled
if(UNITTEST)
  add_subdirectory(unittest)
endif()
