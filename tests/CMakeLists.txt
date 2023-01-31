cmake_minimum_required(VERSION 3.21)
set (PROJECT_NAME_TEST ${PROJECT_NAME}_tests)
project(${PROJECT_NAME_TEST}
        VERSION 0.1
        DESCRIPTION "Testing Quote"
        LANGUAGES CXX)

find_package(GTest REQUIRED)
if(GTest_FOUND)
    message(STATUS "FOUND GTest, including and linking...")
    include_directories(${GTEST_INCLUDE_DIRS})
else()
    message(STATUS "NOT FOUND GTest")
endif()

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
endif (APPLE)


add_executable(${PROJECT_NAME_TEST}
        tests_all.cpp
        tests_ctors_and_assignments.cpp
        tests_comparison.cpp
        tests_arithmetics.cpp
        tests_precision.cpp
        )

message(STATUS "Linking test in Debug mode...")
target_link_libraries(${PROJECT_NAME_TEST}
        ${GTEST_LIBRARIES}
        Boost::boost
        pthread
        fin_quote
        )