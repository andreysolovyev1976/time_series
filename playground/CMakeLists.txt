cmake_minimum_required(VERSION 3.21)

set (PROJECT_NAME_PLAYGROUND ${PROJECT_NAME}_playground)
project(${PROJECT_NAME_PLAYGROUND}
        VERSION 0.1
        DESCRIPTION "Fin Value temporary playground"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic -Werror -Wreturn-std-move -Wno-c99-extensions -o")
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

add_executable(${PROJECT_NAME_PLAYGROUND} playground.cpp)
target_link_libraries(${PROJECT_NAME_PLAYGROUND}
        pthread
        Boost::boost
        #        fin_quote
        )
