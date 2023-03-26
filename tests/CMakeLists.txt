cmake_minimum_required(VERSION 3.21)
set (PROJECT_NAME_TEST ${PROJECT_NAME}_tests)
project(${PROJECT_NAME_TEST}
        VERSION 0.1
        DESCRIPTION "Testing Timeseries"
        LANGUAGES CXX)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if (APPLE OR LINUX)
    #debug
    add_definitions(-D__GLIBCXX__)

    #Threads not found
    if(NOT Threads_FOUND)
        set(CMAKE_THREAD_LIBS_INIT "-lpthread")
        set(CMAKE_HAVE_THREADS_LIBRARY 1)
        set(CMAKE_USE_WIN32_THREADS_INIT 0)
        set(CMAKE_USE_PTHREADS_INIT 1)
        set(THREADS_PREFER_PTHREAD_FLAG ON)
    endif()
endif ()

if (APPLE)
    set(CMAKE_CXX_FLAGS "-Wno-gnu-zero-variadic-macro-arguments")
endif()

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
endif (APPLE)


find_package (Boost)
if(Boost_FOUND)
    message(STATUS "TESTS FOUND Boost ${Boost_VERSION}, including and linking...")
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREAD ON)
    include_directories(${Boost_INCLUDE_DIRS})
    #    message(STATUS "incl: ${Boost_INCLUDE_DIRS}")
else()
    message(STATUS "NOT FOUND Boost")
endif()


find_package(GTest REQUIRED)
if(GTest_FOUND)
    message(STATUS "TESTS FOUND GTest, including and linking...")
    include_directories(${GTEST_INCLUDE_DIRS})
else()
    message(STATUS "NOT FOUND GTest")
endif()


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


target_compile_options(${PROJECT_NAME_TEST}
        PRIVATE
        -fsanitize=address -g -fno-omit-frame-pointer -fno-optimize-sibling-calls
        -fsanitize=undefined -g -fno-omit-frame-pointer
#        -fsanitize=leak -g
#        -fsanitize=thread -fPIE -pie -g
        )
target_link_options(${PROJECT_NAME_TEST}
        PRIVATE
        -fsanitize=address
        -fsanitize=undefined
#        -fsanitize=leak
#        -fsanitize=thread
        )

target_link_libraries(${PROJECT_NAME_TEST}
        Boost::boost
        GTest::GTest
        pthread
        fin_data_time_series
        )