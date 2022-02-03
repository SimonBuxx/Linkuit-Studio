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

UndoType UndoDeleteType::Type() const
{
    return UndoType::DEL;
}

const std::vector<BaseComponent*>& UndoDeleteType::Components() const
{
    return mComponents;
}
