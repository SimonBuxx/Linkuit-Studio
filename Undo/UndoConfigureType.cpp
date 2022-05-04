#include "UndoConfigureType.h"

UndoConfigureType::UndoConfigureType(std::shared_ptr<undo::ConfigChangedData> pData):
    mData(pData)
{}

undo::Type UndoConfigureType::Type() const
{
    return undo::Type::CONFIGURE;
}

std::shared_ptr<undo::ConfigChangedData> UndoConfigureType::Data(void)
{
    return mData;
}
