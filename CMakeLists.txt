# Author : Animesh Trivedi

cmake_minimum_required(VERSION 3.9)

project (rdma-example)

include(GNUInstallDirs) # moodycamel SPSC-Queue

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Release) # or debug if you need to use gdb on it.

# Some how g++ has better performance than clang++. Of course I don't know all flags for clang++.
set(CMAKE_CXX_COMPILER g++) # or clang++
#set(CMAKE_CXX_COMPILER clang++) # or clang++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Ofast -ffast-math -funroll-loops -march=native") # get crazy here
# add_compile_options("-static-libstdc++")
# add_compile_options("-std=c++17")
#set(CMAKE_CXX_FLAGS "-O0  -ggdb")

find_library(IBVERBS_LIBRARY ibverbs HINTS /usr/local/lib)
find_library(RDMACM_LIBRARY rdmacm HINTS /usr/local/lib)
link_libraries(pthread ${IBVERBS_LIBRARY} ${RDMACM_LIBRARY})
include_directories("${PROJECT_SOURCE_DIR}" "/usr/local/include/")


set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

cmake_host_system_information(RESULT _host_name QUERY HOSTNAME)
message("-- _host_name variable is set to: " ${_host_name})
if( ${_host_name} STREQUAL "hajime" )
    message("-- Hajime Build")
    ### ONLY FOR HAJIME ###
    add_executable(rdma_simple_hajime ${PROJECT_SOURCE_DIR}/rdma_simple.cpp ${PROJECT_SOURCE_DIR}/rdma_simple.cpp)
    add_executable(rdma_client_hajime ${PROJECT_SOURCE_DIR}/rdma_client.cpp ${PROJECT_SOURCE_DIR}/rdma_client.cpp)
    add_executable(rdma_client_esg_hajime ${PROJECT_SOURCE_DIR}/rdma_client_esg.cpp ${PROJECT_SOURCE_DIR}/rdma_client_esg.cpp)
    add_executable(rdma_server_hajime ${PROJECT_SOURCE_DIR}/rdma_server.cpp ${PROJECT_SOURCE_DIR}/rdma_server.cpp)
else ()
    ### GENERIC ###
    message("-- Generic Build")
    add_executable(rdma_simple ${PROJECT_SOURCE_DIR}/rdma_simple.cpp ${PROJECT_SOURCE_DIR}/rdma_simple.cpp)
    add_executable(rdma_client ${PROJECT_SOURCE_DIR}/rdma_client.cpp ${PROJECT_SOURCE_DIR}/rdma_client.cpp)
    add_executable(rdma_client_esg ${PROJECT_SOURCE_DIR}/rdma_client_esg.cpp ${PROJECT_SOURCE_DIR}/rdma_client_esg.cpp)
    add_executable(rdma_server ${PROJECT_SOURCE_DIR}/rdma_server.cpp ${PROJECT_SOURCE_DIR}/rdma_server.cpp)
endif()


