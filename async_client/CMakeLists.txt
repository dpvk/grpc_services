cmake_minimum_required(VERSION 2.8.12)
project(gRPCStreamer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(cmake/grpc_gen.cmake)


add_subdirectory(proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/proto)
add_subdirectory(client)
add_subdirectory(server)