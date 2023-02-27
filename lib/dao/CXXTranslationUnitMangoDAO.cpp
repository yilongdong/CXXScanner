#include <google/protobuf/util/json_util.h>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <mongocxx/exception/error_code.hpp>
#include <mongocxx/exception/logic_error.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <mongocxx/exception/server_error_code.hpp>
#include <bsoncxx/json.hpp>
#include "dao/CXXTranslationUnitMongoDAO.h"
#include "dao/MongoConnectionPool.h"
#include "utility/log.h"

namespace CXXScanner::dao {
    using ConnPool = CXXScanner::dao::ConnectionPool;

    CXXTranslationUnitMangoDAO::CXXTranslationUnitMangoDAO() {
        ConnPool::instance()
            .username("xxx")
            .password("xxx")
            .host({127,0,0,1})
            .port(27017)
            .auth_database("admin");
    }

    void CXXTranslationUnitMangoDAO::createOne(CXXScanner::dao::po::CXXTranslationUnit const& TranslationUnitDO) {
        std::string TUJsonStr;
        auto state = google::protobuf::util::MessageToJsonString(TranslationUnitDO, &TUJsonStr);
        if (!state.ok()) {
            LOG_ERROR("{}", state.ToString());
        }
        auto docValue = bsoncxx::from_json(TUJsonStr);
        auto entry = ConnPool::instance().acquire();
        mongocxx::client& conn = *entry;
        conn["test"]["TUs"].drop();
        mongocxx::database testDB = conn["test"];
        mongocxx::collection TUCollection = testDB["TUs"];

        try {
            TUCollection.insert_one(docValue.view());
        } catch (const mongocxx::bulk_write_exception& e) {
            if (e.raw_server_error()) {
                LOG_ERROR("Raw server error:{}", bsoncxx::to_json(*(e.raw_server_error())));
            }
            else {
                LOG_ERROR("Adding a document whose _id is already present throws:{}",e.what());
            }
            return;
        }
        LOG_TRACE("success insert");
    }

    void CXXTranslationUnitMangoDAO::findOneById(model::Element::id_t id, CXXScanner::dao::po::CXXTranslationUnit & TranslationUnitDO) {
        auto entry = ConnPool::instance().acquire();
        mongocxx::client& conn = *entry;
        mongocxx::database testDB = conn["test"];
        mongocxx::collection TUCollection = testDB["TUs"];

        std::optional<bsoncxx::document::value> maybe_result = TUCollection.find_one({});
        if(maybe_result.has_value()) {
            bsoncxx::document::value result = maybe_result.value();
            auto jsonStr = bsoncxx::to_json(result.view());
            auto state = google::protobuf::util::JsonStringToMessage(jsonStr, &TranslationUnitDO);
            if (state.ok()) {
                LOG_ERROR("{}", state.ToString());
            }
        }
    }

    template<typename TUModelFwdIter>
    void CXXTranslationUnitMangoDAO::createMany(TUModelFwdIter first, TUModelFwdIter last) {
        std::vector<bsoncxx::document::view> documents;
        for (auto iter = first; iter != last; ++iter) {
            CXXScanner::dao::po::CXXTranslationUnit const& TU = *iter;
            std::string jsonStr;
            auto state = google::protobuf::util::MessageToJsonString(TU, &jsonStr);
            if (!state.ok()) {
                LOG_ERROR("{}", state.ToString());
            }
            documents.push_back(bsoncxx::from_json(jsonStr));
        }

        auto entry = ConnPool::instance().acquire();
        mongocxx::client& conn = *entry;
        mongocxx::database testDB = conn["test"];
        mongocxx::collection TUCollection = testDB["TUs"];
        TUCollection.insert_many(documents);
    }

    template<typename ElementIdFwdIter, typename InserterIter>
    void CXXTranslationUnitMangoDAO::findManyByIds(ElementIdFwdIter first, ElementIdFwdIter last, InserterIter dst) {
        auto entry = ConnPool::instance().acquire();
        mongocxx::client& conn = *entry;
        mongocxx::database testDB = conn["test"];
        mongocxx::collection TUCollection = testDB["TUs"];

        mongocxx::cursor cursor = TUCollection.find({});
        for (auto doc : cursor) {
            auto state = google::protobuf::util::JsonStringToMessage(bsoncxx::to_json(doc), &(*dst));
            if (state.ok()) {
                LOG_ERROR("{}", state.ToString());
            }
        }
    }
}