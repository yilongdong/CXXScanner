#pragma once
#include "dao/CXXTranslationUnitMongoDAO.h"
#include "dao/CXXModelToPersistantObject.h"
#include "model/CXXTranslationUnit.h"
#include "utility/log.h"
#include "CXXTranslationUnitPO.pb.h"

namespace CXXScanner::reporter {

    class TUReporter {
    public:

        TUReporter& loadTranslationUnit(CXXScanner::model::CXXTranslationUnit const& TU) {
            convertor::CXXModelToPersistantObject convertor;
            if (auto PO = convertor.convert(TU)) {
                m_persistantObjectsOfTUs.push_back(std::move(*(PO.release())));
            }
            return *this;
        }

        void report() {
            for (auto const& TU : m_persistantObjectsOfTUs) {
                m_dao.createOne(TU);
            }
        }
    private:
        std::vector<CXXScanner::dao::po::CXXTranslationUnit> m_persistantObjectsOfTUs;
        CXXScanner::dao::CXXTranslationUnitMangoDAO m_dao;
    };

}
