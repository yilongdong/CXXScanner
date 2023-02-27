#pragma once
#include "dao/CXXTranslationUnitDAO.hpp"
#include "CXXTranslationUnitPO.pb.h"

namespace CXXScanner::dao {
    class CXXTranslationUnitMangoDAO : public CXXTranslationUnitDAOInterface<CXXTranslationUnitMangoDAO> {
    public:
        CXXTranslationUnitMangoDAO();
        // 同步接口
        void createOne(CXXScanner::dao::po::CXXTranslationUnit const& TranslationUnitDO);
        void findOneById(model::Element::id_t id, CXXScanner::dao::po::CXXTranslationUnit& TranslationUnitDO);
        template<typename TUModelFwdIter>
        void createMany(TUModelFwdIter first, TUModelFwdIter last);
        template<typename ElementIdFwdIter, typename InserterIter>
        void findManyByIds(ElementIdFwdIter first, ElementIdFwdIter last, InserterIter dst);
    };
}