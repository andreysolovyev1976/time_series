cmake_minimum_required(VERSION 3.21)

set (PROJECT_NAME_TIME_SERIES ${PROJECT_NAME}_time_series)
project(${PROJECT_NAME_TIME_SERIES})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fconcepts-diagnostics-depth=3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -D_LIBCPP_DEBUG=0")

include_directories(third_part/date_time/include)
include_directories(third_part/cds)

add_library(${PROJECT_NAME_TIME_SERIES} quote.cpp)
target_link_libraries(${PROJECT_NAME_TIME_SERIES}
        pthread
        )
target_include_directories (${PROJECT_NAME_TIME_SERIES} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})