#include <memory>
#include "CXXTranslationUnitPO.pb.h"
#include "model/CXXTranslationUnit.h"

namespace CXXScanner::convertor {
    class CXXModelToPersistantObject {
    public:
        std::unique_ptr<dao::po::CXXTranslationUnit> convert(model::CXXTranslationUnit const& tuModel);
        std::unique_ptr<dao::po::CXXClass> convert(model::CXXClass const& clsModel);
        std::unique_ptr<dao::po::CXXInclusion> convert(model::CXXInclusion const& inclusionModel);
        std::unique_ptr<dao::po::CXXMethod> convert(model::CXXMethod const& methodModel);
        std::unique_ptr<dao::po::CXXField> convert(model::CXXField const& fieldModel);
        std::unique_ptr<dao::po::CXXParam> convert(model::CXXFunction::CXXParam const& paramModel);
        std::unique_ptr<dao::po::Location> convert(model::SourceLocation const& locationModel);
        std::unique_ptr<dao::po::CXXType> convert(model::CXXType const& typeModel);
        dao::po::CXXAccess convert(model::CXXAccess const& access);
        std::unique_ptr<dao::po::CXXBase> convert(model::CXXBase const& baseModel);
    };
}