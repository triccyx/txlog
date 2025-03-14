cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(example
    VERSION 1.0.0
    DESCRIPTION "Log library example"
    LANGUAGES C CXX
)

add_executable(log_example main.cpp)

target_link_libraries(log_example PRIVATE txlog)

target_include_directories(log_example PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)
