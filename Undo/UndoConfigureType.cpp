#include "UndoConfigureType.h"

UndoConfigureType::UndoConfigureType(const std::shared_ptr<undo::ConfigChangedData>& pData):
    mData(pData)
{}

undo::Type UndoConfigureType::Type() const
{
    return undo::Type::CONFIGURE;
}

std::shared_ptr<undo::ConfigChangedData> UndoConfigureType::Data()
{
    return mData;
}
