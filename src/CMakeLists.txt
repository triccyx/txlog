cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(txlog
    VERSION 1.0.0
    DESCRIPTION "Log library"
    LANGUAGES C CXX
)

configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

set(SOURCE
    logDepot.h
    logConfig.h
    logDepot.cpp
    logConfig.cpp
    log.h
    log.cpp
    line.h
    line.cpp
    logconst.h
    config.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

if(X86)
    set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

if(CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    target_link_libraries(${PROJECT_NAME} PUBLIC magic_enum::magic_enum)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

