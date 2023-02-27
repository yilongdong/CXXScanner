find_package(Protobuf CONFIG REQUIRED)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS CXXTranslationUnitPO.proto)
add_library(CXXTranslationUnitPO ${PROTO_SRCS} ${PROTO_HDRS})
add_library(CXXScanner::proto ALIAS CXXTranslationUnitPO)
target_include_directories(CXXTranslationUnitPO
        PUBLIC
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        )
target_link_libraries(CXXTranslationUnitPO
        PUBLIC
        ${Protobuf_LIBRARIES}
        )

find_package(mongocxx CONFIG REQUIRED)
find_package(fmt)
add_library(MongoConnPool MongoConnectionPool.cpp)
target_link_libraries(MongoConnPool
        PUBLIC
        $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>
        $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
        fmt::fmt)

add_library(ModelToPOConvertor CXXModelToPersistantObject.cpp ${PROJECT_SOURCE_DIR}/include/dao/CXXModelToPersistantObject.h)
#target_link_libraries(ModelToPOConvertor)
#target_include_directories(ModelToPOConvertor
#        PUBLIC
#        ${CMAKE_CURRENT_BINARY_DIR}
#        )
target_link_libraries(ModelToPOConvertor
        PUBLIC
        CXXScanner::proto
        )
include_directories(${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

add_library(DataAccessObject CXXTranslationUnitMangoDAO.cpp)
add_library(CXXScanner::dao ALIAS DataAccessObject)
target_link_libraries(DataAccessObject PUBLIC MongoConnPool ModelToPOConvertor)

