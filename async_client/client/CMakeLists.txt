cmake_minimum_required(VERSION 2.8.12)

add_executable(client client.cpp ${PROTO_SRCS} ${GRPC_SRCS} )
target_link_libraries(client apilib ${CONAN_LIBS})
