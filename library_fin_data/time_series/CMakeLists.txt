cmake_minimum_required(VERSION 3.21)

set (PROJECT_NAME_TIME_SERIES ${PROJECT_NAME}_time_series)
project(${PROJECT_NAME_TIME_SERIES})

include_directories(third_part/date_time/include)
include_directories(third_part/cds)

add_library(${PROJECT_NAME_TIME_SERIES} quote.cpp)
target_link_libraries(${PROJECT_NAME_TIME_SERIES}
        Boost::boost
        pthread
        )
target_include_directories (${PROJECT_NAME_TIME_SERIES} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})