#include "UndoMoveType.h"

UndoMoveType::UndoMoveType(std::vector<IBaseComponent*> &pMovedComponents, QPointF pOffset):
    mMovedComponents(pMovedComponents),
    mOffset(pOffset)
{}

UndoMoveType::UndoMoveType(std::vector<IBaseComponent*> &pMovedComponents, std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents, QPointF pOffset):
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

undo::Type UndoMoveType::Type() const
{
    return undo::Type::MOVE;
}

const std::vector<IBaseComponent*>& UndoMoveType::MovedComponents() const
{
    return mMovedComponents;
}

const std::vector<IBaseComponent*>& UndoMoveType::AddedComponents() const
{
    return mAddedComponents;
}

const std::vector<IBaseComponent*>& UndoMoveType::DeletedComponents() const
{
    return mDeletedComponents;
}


const QPointF UndoMoveType::Offset() const
{
    return mOffset;
}

