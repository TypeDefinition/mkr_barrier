cmake_minimum_required(VERSION 3.23.2)

project("mkr_barrier")

set(SRC_DIR "src")
file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES true CONFIGURE_DEPENDS
        "${SRC_DIR}/*.h"
        "${SRC_DIR}/*.c"
        "${SRC_DIR}/*.hpp"
        "${SRC_DIR}/*.cpp")
add_library(${PROJECT_NAME} ${SRC_FILES})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
target_link_libraries("mkr_barrier" Threads::Threads)