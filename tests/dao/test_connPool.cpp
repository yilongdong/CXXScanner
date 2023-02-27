#include <gtest/gtest.h>
#include "dao/MongoConnectionPool.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
TEST(dao, connPool) {
    CXXScanner::dao::ConnectionPool::instance()
        .username("xxx")
        .password("xxx")
        .host({127,0,0,1})
        .port(27017)
        .auth_database("admin").connect();

    auto entry = CXXScanner::dao::ConnectionPool::instance().acquire();
    auto& client = *entry;
    auto collection = client["test"]["testConnPool"];
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
            << "name" << "MongoDB"
            << "type" << "database"
            << "count" << 1
            << "versions" << bsoncxx::builder::stream::open_array
            << "v3.2" << "v3.0" << "v2.6"
            << close_array
            << "info" << bsoncxx::builder::stream::open_document
            << "x" << 203
            << "y" << 102
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize;

    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
            collection.insert_one(doc_value.view());
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}