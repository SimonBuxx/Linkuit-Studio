#include "UndoAddType.h"

UndoAddType::UndoAddType(std::vector<BaseComponent*> &pAddedComponents):
    mAddedComponents(pAddedComponents),
    mDeletedComponents()
{}

UndoAddType::UndoAddType(std::vector<BaseComponent*> &pAddedComponents, std::vector<BaseComponent*> &pDeletedComponents):
    mAddedComponents(pAddedComponents),
    mDeletedComponents(pDeletedComponents)
{}

UndoType UndoAddType::Type() const
{
    return UndoType::ADD;
}

const std::vector<BaseComponent*>& UndoAddType::AddedComponents() const
{
    return mAddedComponents;
}

const std::vector<BaseComponent*>& UndoAddType::DeletedComponents() const
{
    return mDeletedComponents;
}
