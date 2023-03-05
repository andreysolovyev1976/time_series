cmake_minimum_required(VERSION 3.21)
set (PROJECT_NAME fin_data)
project(${PROJECT_NAME}
        VERSION 0.1
        DESCRIPTION "Financial time series project"
        LANGUAGES CXX)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()


set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    set(CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic -Werror -Wreturn-std-move -Wno-missing-field-initializers -Wno-gnu-zero-variadic-macro-arguments -o")
else()
    set(CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic -Werror -Wno-c99-extensions")
endif()

set(CMAKE_CXX_FLAGS_DEBUG " -fsanitize=undefined -g")
set(CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -g -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_CXX_FLAGS_DEBUG " -fsanitize=thread -g")
set(CMAKE_CXX_FLAGS_RELEASE " -O3")


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

include_directories(library_fin_data)
include_directories(library_fin_data/third_part/date_time/include)
include_directories(library_fin_data/third_part/cds)

add_subdirectory(library_fin_data/time_series)
enable_testing()
add_subdirectory(tests)
add_subdirectory(playground)
