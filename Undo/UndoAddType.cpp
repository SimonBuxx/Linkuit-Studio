#include "UndoAddType.h"

UndoAddType::UndoAddType(std::vector<BaseComponent*> &pAddedComponents):
    mComponents(pAddedComponents)
{}

UndoType UndoAddType::Type() const
{
    return UndoType::ADD;
}

const std::vector<BaseComponent*>& UndoAddType::Components() const
{
    return mComponents;
}
