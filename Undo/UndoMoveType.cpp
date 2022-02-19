#include "UndoMoveType.h"

UndoMoveType::UndoMoveType(std::vector<BaseComponent*> &pMovedComponents, QPointF pOffset):
    mMovedComponents(pMovedComponents),
    mOffset(pOffset)
{}

UndoMoveType::UndoMoveType(std::vector<BaseComponent*> &pMovedComponents, std::vector<BaseComponent*> &pAddedComponents, std::vector<BaseComponent*> &pDeletedComponents, QPointF pOffset):
    mMovedComponents(pMovedComponents),
    mAddedComponents(pAddedComponents),
    mDeletedComponents(pDeletedComponents),
    mOffset(pOffset)
{}

UndoMoveType::~UndoMoveType()
{
    for (auto& comp : mDeletedComponents)
    {
        delete comp;
    }
}

Undo::Type UndoMoveType::Type() const
{
    return Undo::Type::MOVE;
}

const std::vector<BaseComponent*>& UndoMoveType::MovedComponents() const
{
    return mMovedComponents;
}

const std::vector<BaseComponent*>& UndoMoveType::AddedComponents() const
{
    return mAddedComponents;
}

const std::vector<BaseComponent*>& UndoMoveType::DeletedComponents() const
{
    return mDeletedComponents;
}


const QPointF UndoMoveType::Offset() const
{
    return mOffset;
}

