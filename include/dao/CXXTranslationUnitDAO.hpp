#pragma once
#include "model/CXXTranslationUnit.h"
//#include "CXXTranslationUnitPO.pb.h"

namespace CXXScanner::dao {

    template <typename CXXTranslationUnitDAOImpl>
    class CXXTranslationUnitDAOInterface {
    protected:
        CXXTranslationUnitDAOInterface() = default;
        ~CXXTranslationUnitDAOInterface() = default;
    public:
        // 同步接口
        void createOne(model::CXXTranslationUnit const& TUModel) {
            static_cast<CXXTranslationUnitDAOImpl*>(this)->createOne(TUModel);
        }
        void findOneById(model::Element::id_t id, model::CXXTranslationUnit& TranslationUnitDO) {
            static_cast<CXXTranslationUnitDAOImpl*>(this)->findOneById(id, TranslationUnitDO);
        }
        template<typename TUModelFwdIter>
        void createMany(TUModelFwdIter first, TUModelFwdIter last) {
            static_cast<CXXTranslationUnitDAOImpl*>(this)->createMany(first, last);
        }
        template<typename ElementIdFwdIter, typename InserterIter>
        void findManyByIds(ElementIdFwdIter first, ElementIdFwdIter last, InserterIter dst) {
            static_cast<CXXTranslationUnitDAOImpl*>(this)->findManyByIds(first, last, dst);
        }
    };
}
