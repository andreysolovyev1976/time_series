cmake_minimum_required(VERSION 3.21)

set (PROJECT_NAME_PLAYGROUND ${PROJECT_NAME}_playground)
project(${PROJECT_NAME_PLAYGROUND}
        VERSION 0.1
        DESCRIPTION "Fin Value temporary playground"
        LANGUAGES CXX)

set(CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE " -O3")


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


find_package (Boost)
if(Boost_FOUND)
    message(STATUS "PLAYGROUND FOUND Boost ${Boost_VERSION}, including and linking...")
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREAD ON)
    include_directories(${Boost_INCLUDE_DIRS})
    #    message(STATUS "incl: ${Boost_INCLUDE_DIRS}")
else()
    message(STATUS "NOT FOUND Boost")
endif()

add_executable(${PROJECT_NAME_PLAYGROUND} playground.cpp)
target_compile_options(${PROJECT_NAME_PLAYGROUND}
        PRIVATE
        -fsanitize=address -g -fno-omit-frame-pointer -fno-optimize-sibling-calls
        -fsanitize=undefined -g -fno-omit-frame-pointer
#        -fsanitize=leak -g
#        -fsanitize=thread -fPIE -pie -g
        )
target_link_options(${PROJECT_NAME_PLAYGROUND}
        PRIVATE
        -fsanitize=address
        -fsanitize=undefined
#        -fsanitize=leak
#        -fsanitize=thread
        )
target_link_libraries(${PROJECT_NAME_PLAYGROUND}
        pthread
        Boost::boost
#        fin_quote
        )


