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

Undo::Type UndoDeleteType::Type() const
{
    return Undo::Type::DEL;
}

const std::vector<IBaseComponent*>& UndoDeleteType::Components() const
{
    return mComponents;
}
