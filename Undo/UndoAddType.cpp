#include "UndoAddType.h"

UndoAddType::UndoAddType(std::vector<BaseComponent*> &pAddedComponents):
    mAddedComponents(pAddedComponents),
    mDeletedComponents()
{}

UndoAddType::UndoAddType(std::vector<BaseComponent*> &pAddedComponents, std::vector<BaseComponent*> &pDeletedComponents):
    mAddedComponents(pAddedComponents),
    mDeletedComponents(pDeletedComponents)
{}

UndoAddType::~UndoAddType()
{
    for (auto& comp : mDeletedComponents)
    {
        delete comp;
    }
}

Undo::Type UndoAddType::Type() const
{
    return Undo::Type::ADD;
}

const std::vector<BaseComponent*>& UndoAddType::AddedComponents() const
{
    return mAddedComponents;
}

const std::vector<BaseComponent*>& UndoAddType::DeletedComponents() const
{
    return mDeletedComponents;
}
