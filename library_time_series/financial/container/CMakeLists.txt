cmake_minimum_required(VERSION 3.23)
project(pmr_container)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fconcepts-diagnostics-depth=3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -D_LIBCPP_DEBUG=1")

#Threads not found
if(APPLE)
    if(NOT Threads_FOUND)
        set(CMAKE_THREAD_LIBS_INIT "-lpthread")
        set(CMAKE_HAVE_THREADS_LIBRARY 1)
        set(CMAKE_USE_WIN32_THREADS_INIT 0)
        set(CMAKE_USE_PTHREADS_INIT 1)
        set(THREADS_PREFER_PTHREAD_FLAG ON)
    endif()
endif()


find_package(benchmark REQUIRED)
if(benchmark_FOUND)
    message(STATUS " <===> FOUND BENCHMARK, including and linking...")

    add_executable(${PROJECT_NAME}_bench bench.cpp)
    target_compile_options(${PROJECT_NAME}_bench
            PRIVATE
            -Wno-uninitialized
            )
    target_link_libraries(${PROJECT_NAME}_bench
            benchmark::benchmark
            pthread
            )
else()
    message(STATUS " <===> NOT FOUND BENCHMARK")
endif()


find_package(GTest REQUIRED)
if(GTest_FOUND)
    message(STATUS " <===> FOUND GTest, including and linking...")
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME}_tests tests.cpp)
    target_compile_options(${PROJECT_NAME}_tests
            PRIVATE
            -Wno-uninitialized
            -fsanitize=address -g -fno-omit-frame-pointer -fno-optimize-sibling-calls
            -fsanitize=undefined -g -fno-omit-frame-pointer
#            -fsanitize=leak -g -fno-omit-frame-pointer
#            -fsanitize=thread -fPIE -pie -g
            )
    target_link_options(${PROJECT_NAME}_tests
            PRIVATE
            -fsanitize=address
            -fsanitize=undefined
#            -fsanitize=leak
#            -fsanitize=thread
            )
    target_link_libraries(${PROJECT_NAME}_tests
            GTest::GTest
            pthread
            )
else()
    message(STATUS " <===> NOT FOUND GTest")
endif()
