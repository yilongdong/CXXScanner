find_package(Protobuf CONFIG REQUIRED)
file(GLOB DumpModelProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${DumpModelProtoFiles})
add_library(model
        ${PROTO_SRCS} ${PROTO_HDRS})
add_library(CXXScanner::model ALIAS model)
target_include_directories(model
        PUBLIC ${Protobuf_INCLUDE_DIRS}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/../"
        )
target_link_libraries(model
        PUBLIC ${Protobuf_LIBRARIES}
        )
