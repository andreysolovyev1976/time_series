cmake_minimum_required(VERSION 3.21)
set (PROJECT_NAME_TEST ${PROJECT_NAME}_tests)
project(${PROJECT_NAME_TEST}
        VERSION 0.1
        DESCRIPTION "Testing Timeseries"
        LANGUAGES CXX)

if (APPLE)
    set(CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic -Werror -Wreturn-std-move -Wno-c99-extensions -o")
    set(CMAKE_CXX_STANDARD 17)
else()
    set(CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_FLAGS_RELEASE " -O3")


find_package (Boost)
if(Boost_FOUND)
    message(STATUS "FOUND Boost, including and linking...")
    #    message(STATUS "${Boost_VERSION}")
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREAD ON)
    include_directories(${Boost_INCLUDE_DIRS})
    #    message(STATUS "incl: ${Boost_INCLUDE_DIRS}")
else()
    message(STATUS "NOT FOUND Boost")
endif()


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

        tests_basics_floating_point_comparison.cpp
        tests_basics_timestamp.cpp
        tests_basics_types_requirements.cpp
        tests_basics_itertools.cpp
        tests_basics_joins.cpp

        tests_value_ctors.cpp
        tests_value_comparison.cpp
        tests_value_conversion_operators.cpp
        tests_value_arithmetics.cpp

        tests_element_ctors.cpp
        tests_element_comparison.cpp
        tests_element_conversion_operators.cpp
        tests_element_arithmetics.cpp
        tests_element_fun_apply.cpp

        tests_serie_container.cpp
        tests_serie_itertools.cpp
        tests_serie_joins.cpp

        tests_serie_fun_apply.cpp)

message(STATUS "Linking test in Debug mode...")
target_link_libraries(${PROJECT_NAME_TEST}
        Boost::boost
        ${GTEST_LIBRARIES}
        pthread
        fin_data_time_series
        )