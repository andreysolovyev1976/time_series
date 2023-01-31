cmake_minimum_required(VERSION 3.21)

set (PROJECT_NAME_PLAYGROUND ${PROJECT_NAME}_playground)
project(${PROJECT_NAME_PLAYGROUND}
        VERSION 0.1
        DESCRIPTION "Fin Value temporary playground"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE " -O3")

add_executable(${PROJECT_NAME_PLAYGROUND} playground.cpp)
target_link_libraries(${PROJECT_NAME_PLAYGROUND}
        Boost::boost
        pthread
#        fin_quote
        )
