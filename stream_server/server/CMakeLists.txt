cmake_minimum_required(VERSION 2.8.12)

add_executable(server server.cpp ${PROTO_SRCS} ${GRPC_SRCS} )
target_link_libraries(server apilib ${CONAN_LIBS})