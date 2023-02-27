#include "dao/CXXModelToPersistantObject.h"

std::unique_ptr<CXXScanner::dao::po::CXXTranslationUnit> CXXScanner::convertor::CXXModelToPersistantObject::convert(
        const CXXScanner::model::CXXTranslationUnit &tuModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXTranslationUnit>();
    PO->set_path(tuModel.path.u8string());
    PO->set__id(tuModel.id());
    for (auto const& classModel : tuModel.classes) {
        PO->mutable_classes()->AddAllocated(convert(classModel).release());
    }
    for(auto const& inclusionModel : tuModel.inclusions) {
        PO->mutable_inclusions()->AddAllocated(convert(inclusionModel).release());
    }
    return PO;
}

std::unique_ptr<CXXScanner::dao::po::CXXClass>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXClass &clsModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXClass>();
    PO->set_name(clsModel.name);
    for (auto const& field : clsModel.fields) {
        PO->mutable_fields()->AddAllocated(convert(field).release());
    }
    for (auto const& method : clsModel.methods) {
        PO->mutable_methods()->AddAllocated(convert(method).release());
    }
    for (auto const& base : clsModel.bases) {
        PO->mutable_bases()->AddAllocated(convert(base).release());
    }
    PO->set_allocated_location(convert(clsModel.loc).release());
    return PO;
}

std::unique_ptr<CXXScanner::dao::po::CXXInclusion>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXInclusion &inclusionModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXInclusion>();
    PO->set_path(inclusionModel.filename);
    PO->set_allocated_location(convert(inclusionModel.loc).release());
    return PO;
}

std::unique_ptr<CXXScanner::dao::po::CXXMethod>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXMethod &methodModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXMethod>();
    PO->set_name(methodModel.name);
    PO->set_access(convert(methodModel.access));
    PO->set_allocated_return_type(convert(methodModel.return_type).release());
    for (auto const& paramModel : methodModel.params) {
        PO->mutable_params()->AddAllocated(convert(paramModel).release());
    }
    return PO;
}

std::unique_ptr<CXXScanner::dao::po::CXXField>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXField &fieldModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXField>();
    PO->set_name(fieldModel.name);
    PO->set_access(convert(fieldModel.access));
    PO->set_allocated_type(convert(fieldModel.type).release());
    return PO;
}

std::unique_ptr<CXXScanner::dao::po::CXXParam>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXFunction::CXXParam &paramModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXParam>();
    PO->set_name(paramModel.name);
    PO->set_allocated_type(convert(paramModel.type).release());
    PO->set_default_value(paramModel.default_value);
    return PO;
}

std::unique_ptr<CXXScanner::dao::po::Location>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::SourceLocation &locationModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::Location>();
    PO->set_line(locationModel.line);
    PO->set_column(locationModel.column);
    PO->set_path(locationModel.path);
    return PO;
}

CXXScanner::dao::po::CXXAccess
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXAccess &access) {

    switch (access) {
        case (CXXScanner::model::CXXAccess::AC_PUBLIC): {
            return CXXScanner::dao::po::CXXAccess::AC_PUBLIC;
        }
        case (CXXScanner::model::CXXAccess::AC_PROTECTED): {
            return CXXScanner::dao::po::CXXAccess::AC_PROTECTED;
        }
        case (CXXScanner::model::CXXAccess::AC_PRIVATE): {
            return CXXScanner::dao::po::CXXAccess::AC_PRIVATE;
        }
        default : {
            return CXXScanner::dao::po::CXXAccess::AC_UNKNOWN;
        }
    }
}

std::unique_ptr<CXXScanner::dao::po::CXXType>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXType &typeModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXType>();
    PO->set_name(typeModel.name);
    return PO;
}

std::unique_ptr<CXXScanner::dao::po::CXXBase>
CXXScanner::convertor::CXXModelToPersistantObject::convert(const CXXScanner::model::CXXBase &baseModel) {
    auto PO = std::make_unique<CXXScanner::dao::po::CXXBase>();
    PO->set_access(convert(baseModel.access));
    PO->set_allocated_type(convert(baseModel.type).release());
    PO->set_is_virtual(baseModel.is_virtual);
    return PO;
}
