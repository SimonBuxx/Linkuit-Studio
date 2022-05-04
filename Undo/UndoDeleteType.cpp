#include "UndoDeleteType.h"

UndoDeleteType::UndoDeleteType(std::vector<IBaseComponent*> &pDeletedComponents):
    mComponents(pDeletedComponents)
{}

UndoDeleteType::~UndoDeleteType()
{
    for (auto& comp : mComponents)
    {
        delete comp;
    }
}

undo::Type UndoDeleteType::Type() const
{
    return undo::Type::DEL;
}

const std::vector<IBaseComponent*>& UndoDeleteType::Components() const
{
    return mComponents;
}
