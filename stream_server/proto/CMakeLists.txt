cmake_minimum_required(VERSION 2.8.12)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
message(STATUS "Using Protobuf ${Protobuf_VERSION}")


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS api.proto  )
protobuf_generate_grpc_cpp(GRPC_SRCS GRPC_HDRS api.proto  )



add_library(apilib STATIC ${PROTO_SRCS} ${GRPC_SRCS} )

