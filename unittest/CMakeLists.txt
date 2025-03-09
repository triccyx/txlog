cmake_minimum_required(VERSION 3.10.2)
project(unittest
        VERSION 1.0.0
        DESCRIPTION "Unittest for txlog"
        LANGUAGES CXX C)

set(SOURCES gtest_main.cc
        test/logUT.cpp
        test/logThreadUT.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
        gmock
        gtest
        gcov
        txlog
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

include(GoogleTest)

add_test(NAME UnitTests COMMAND ${PROJECT_NAME})

add_custom_target(RunUnitTest
        COMMAND ${PROJECT_NAME}
        COMMENT " Running ${PROJECT_NAME}... "
)

# #COVERAGE
 set(EXCLUDE_FROM_COVERAGE '/usr/include/*'
            '*/build/*'
            '*/unittest/*'
    )
add_custom_target(EvaluateCoverage
        COMMAND echo "********Coverage clear************"
        COMMAND rm -rf ./coverage_reduced.info ./coverage.info
        COMMAND lcov --zerocounters -d ${CMAKE_SOURCE_DIR}/build/
        COMMAND echo "********Run test************"
        COMMAND make RunUnitTest
        COMMAND echo "********Create coverage************"
        COMMAND lcov -d ${CMAKE_SOURCE_DIR}/build/ -c -o ./coverage.info --rc branch_coverage=1 --rc geninfo_unexecuted_blocks=1 --ignore-errors mismatch --ignore-errors negative
        COMMAND echo "********Remove**************"
        COMMAND lcov -d ${CMAKE_SOURCE_DIR}/build/ --remove coverage.info ${EXCLUDE_FROM_COVERAGE} -o coverage_reduced.info
        COMMAND echo "********Generate************"
        COMMAND genhtml --output-directory ${CMAKE_SOURCE_DIR}/build/UTCoverage --title "txlog Unit Tests Coverage" ./coverage_reduced.info
        COMMAND echo "********End coverage********"
)
add_dependencies(EvaluateCoverage RunUnitTest)

add_custom_target(EvaluateCoverageFast
        COMMAND echo "********Create coverage************"
        COMMAND lcov -d ${CMAKE_SOURCE_DIR}/build/ -c -o ./coverage.info --rc branch_coverage=1 --rc geninfo_unexecuted_blocks=1 --ignore-errors mismatch --ignore-errors negative
        COMMAND echo "********Remove**************"
        COMMAND lcov -d ${CMAKE_SOURCE_DIR}/build/ --remove coverage.info ${EXCLUDE_FROM_COVERAGE} -o coverage_reduced.info
        COMMAND echo "********Generate************"
        COMMAND genhtml --output-directory ${CMAKE_SOURCE_DIR}/build/UTCoverage --title "txlog Unit Tests Coverage" ./coverage_reduced.info
        COMMAND echo "********End coverage********"
)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/configuration DESTINATION ${CMAKE_BINARY_DIR}/unittest)
