cmake_minimum_required(VERSION 3.21)
set (PROJECT_NAME fin_data)
project(${PROJECT_NAME}
        VERSION 0.1
        DESCRIPTION "Financial time series project"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fconcepts-diagnostics-depth=3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -D_LIBCPP_DEBUG=1")


include_directories(library_fin_data)
include_directories(library_fin_data/third_part/date_time/include)
include_directories(library_fin_data/third_part/cds)

add_subdirectory(library_fin_data/time_series)
enable_testing()
add_subdirectory(tests)
add_subdirectory(playground)
