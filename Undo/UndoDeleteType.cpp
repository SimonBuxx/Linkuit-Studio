#include "UndoDeleteType.h"

UndoDeleteType::UndoDeleteType(std::vector<BaseComponent*> &pDeletedComponents):
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

const std::vector<BaseComponent*>& UndoDeleteType::Components() const
{
    return mComponents;
}
