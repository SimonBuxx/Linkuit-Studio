#include "UndoConfigureType.h"

UndoConfigureType::UndoConfigureType(std::shared_ptr<Undo::ConfigChangedData> pData):
    mData(pData)
{}

Undo::Type UndoConfigureType::Type() const
{
    return Undo::Type::CONFIGURE;
}

std::shared_ptr<Undo::ConfigChangedData> UndoConfigureType::Data(void)
{
    return mData;
}
